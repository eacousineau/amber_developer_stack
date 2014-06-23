#ifndef EIGEN_UTILITIES_COMPARE_UTILITIES_H
    #define EIGEN_UTILITIES_COMPARE_UTILITIES_H

#include <Eigen/Dense>

namespace eigen_utilities
{

/**
 * @brief Ensure that two matrices have nans in the same spot.
 * To be used in conjunction with norm_nanless() 
 */
    template<typename Derived>
inline bool nan_compare(const Eigen::MatrixBase<Derived> &A, const Eigen::MatrixBase<Derived> &B)
{
    return ((A.array() == A.array()) == (B.array() == B.array())).all();
}

inline double nan_compare(double a, double b)
{
    return std::isnan(a) == std::isnan(b);
}

/**
 * @brief Zero out all NAN matrix entries
 * @ref http://listengine.tuxfamily.org/lists.tuxfamily.org/eigen/2012/01/msg00020.html
 */
    template<typename Derived>
inline double norm_nanless(const Eigen::MatrixBase<Derived> &X)
{
    Eigen::MatrixXd temp = (X.array() == X.array()).select(X, 0);
    return temp.norm();
}

inline double norm_nanless(double x)
{
    if (std::isnan(x))
        return 0;
    else
        return std::fabs(x);
}

/**
 * @brief Return relative difference 
 * @param A
 * @param B
 * @return 
 *  (B - A) / A,    if (A != 0 & B != 0)
 *  (B - A),        if (A == 0)
 * @todo Also using B == 0 because for it will cause an issue for small values that may not matter
 */
inline double diff_relative_nonzero(double a, double b)
{
    if (a == 0 || b == 0)
        return b - a;
    else
        return (b - a) / a;
}

/**
 * @brief See documentation for scalar version
 */
    template<typename Derived>
inline Eigen::MatrixXd diff_relative_nonzero(const Eigen::MatrixBase<Derived> &A, const Eigen::MatrixBase<Derived> &B)
{
    return ((A.array() != 0) * (B.array() != 0)).select((B - A).array() / A.array(), B - A);
}

}

#endif // EIGEN_UTILITIES_COMPARE_UTILITIES_H
