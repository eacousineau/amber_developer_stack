/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#ifndef EIGEN_UTILITIES_INTERP_UTILITIES_H
    #define EIGEN_UTILITIES_INTERP_UTILITIES_H

#include <Eigen/Dense>

#include <eigen_utilities/assert_size.hpp>

namespace eigen_utilities
{

template<typename T, typename Container>
T find_lower_index(const Container &data, const T &value, bool do_saturate = false)
{
    // Assume sorted
    if (data.size() == 0)
        return -1;
    else
    {
        int index = 0;
        int last = data.size() - 1;
        if (do_saturate)
        {
            const T &min = data[0];
            const T &max = data[last];
            if (value < min)
                return -2;
            if (value > max)
                return -3; // Return this as upper
        }
        while (index + 1 < data.size())
        {
            const T &lower = data[index];
            const T &upper = data[index + 1];
            if (value >= lower && value <= upper)
                return index;
            else
                index += 1;
        }
        return -1;
    }
}

class BilinearInterp
{
public:
    Eigen::VectorXd xs;
    Eigen::VectorXd ys;
    Eigen::MatrixXd zs;
public:
    inline BilinearInterp(const Eigen::VectorXd &x, const Eigen::VectorXd &y, const Eigen::MatrixXd &z)
        : xs(x), ys(y), zs(z)
    {
        check();
    }
    void check()
    {
        common_assert(xs.size() > 0);
        common_assert(ys.size() > 0);
        assert_size_matrix(zs, xs.size(), ys.size());
    }

    inline double eval(double x, double y, bool do_saturate = false) const
    {
        common_assert(xs.size() > 0);
        common_assert(ys.size() > 0);
        assert_size_matrix(zs, xs.size(), ys.size());

        // http://en.wikipedia.org/wiki/Bilinear_interpolation
        if (do_saturate)
        {
            double xmin = xs(0);
            double xmax = xs(xs.size() - 1);
            x = std::max(std::min(x, xmin), xmax);
            double ymin = ys(0);
            double ymax = ys(ys.size() - 1);
            y = std::max(std::min(y, ymin), ymax);
        }
        int x_index = find_lower_index(xs, x);
        int y_index = find_lower_index(ys, y);
        if (x_index == -1 || y_index == -1)
            throw std::runtime_error("Bilinear interpolation values out of range");
        double xl = xs(x_index);
        double xu = xs(x_index + 1);
        double yl = ys(y_index);
        double yu = ys(y_index + 1);
        double q11 = zs(x_index, y_index);
        double q21 = zs(x_index + 1, y_index);
        double q12 = zs(x_index, y_index + 1);
        double q22 = zs(x_index + 1, y_index + 1);
        double r1 = (xu - x) / (xu - xl) * q11 + (x - xl) / (xu - xl) * q21;
        double r2 = (xu - x) / (xu - xl) * q12 + (x - xl) / (xu - xl) * q22;
        double z = (yu - y) / (yu - yl) * r1 + (y - yl) / (yu - yl) * r2;
        return z;
    }
};

}

#endif // EIGEN_UTILITIES_INTERP_UTILITIES_H
