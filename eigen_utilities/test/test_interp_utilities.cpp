/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <vector>
#include <eigen_utilities/interp_utilities.hpp>

using namespace Eigen;
using namespace eigen_utilities;

TEST(eigen_utilities_test, basic_interp)
{
    Eigen::VectorXd xs(2);
    xs << 0, 1;
    Eigen::VectorXd ys(3);
    ys << 0, 1, 2;
    Eigen::MatrixXd zs(2, 3);
    zs <<
         0, 1, 2,
         3, 4, 5;

    EXPECT_EQ(0, find_lower_index(xs, 0));
    EXPECT_EQ(-1, find_lower_index(xs, -1));
    EXPECT_EQ(-1, find_lower_index(xs, 3));
    EXPECT_EQ(1, find_lower_index(ys, 1.5));
    EXPECT_EQ(1, find_lower_index(ys, 2));

//    EXPECT_NO_THROW({
    BilinearInterp interp(xs, ys, zs);
    EXPECT_EQ(0, interp.eval(0, 0));
    EXPECT_EQ(5, interp.eval(1, 2));
    EXPECT_EQ(5, interp.eval(5, 5, true));
    EXPECT_EQ(1.5, interp.eval(0.5, 0));
    EXPECT_EQ(0.5, interp.eval(0, 0.5));
    EXPECT_THROW(interp.eval(-1, -1), std::runtime_error);
//    });
}
