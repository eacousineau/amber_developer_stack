/**
 * @author Eric Cousineau <eacousineau@gmail.com>, member of Dr. Aaron
 * Ames's AMBER Lab
 */
#include <gtest/gtest.h>

#include <vector>
#include <eigen_utilities/container_utilities.hpp>

using namespace Eigen;
using namespace eigen_utilities;

TEST(eigen_utilities_test, select_vector)
{
    Eigen::VectorXd in(5);
    in << 0, 10, 20, 30, 40;
    Eigen::VectorXi indices(3);
    indices << 0, 3, 4;
    
    Eigen::VectorXd expected(indices.size());
    expected << 0, 30, 40;
    {
        Eigen::VectorXd actual(indices.size());
        actual.setZero();
        select_rows(in, indices, actual);
        EXPECT_EQ(expected, actual);
        
        // Test std::vector
        std::vector<double> indices_v;
        std::copy(indices.data(), indices.data() + indices.size(), std::back_inserter(indices_v));
        actual.setZero();
        select_rows(in, indices_v, actual);
        EXPECT_EQ(expected, actual);
    }
    {
        // Test resizing
        Eigen::VectorXd actual;
        EXPECT_THROW(select_rows(in, indices, actual), eigen_utilities::assert_error);
        EXPECT_NO_THROW(select_rows_resize(in, indices, actual));
        EXPECT_EQ(expected, actual);
    }
    
    {
        // Test column selection (note usage of MatrixXd!)
        Eigen::MatrixXd actual;
        select_cols_resize(in.transpose(), indices, actual);
        EXPECT_EQ(expected.transpose(), actual);
    }
}

TEST(eigen_utilities_test, select_matrix)
{
    Eigen::MatrixXd in(3, 3);
    in <<
        0, 10, 20,
        30, 40, 50,
        60, 70, 80;
    
    Eigen::VectorXi rows(2);
    rows << 0, 2;
    Eigen::MatrixXd expected_rows(rows.size(), in.cols());
    expected_rows <<
        0, 10, 20,
        60, 70, 80;
    
    {
        Eigen::MatrixXd actual(expected_rows.rows(), expected_rows.cols());
        actual.setZero();
        select_rows(in, rows, actual);
        EXPECT_EQ(expected_rows, actual);
    }
    {
        // Test resizing
        Eigen::MatrixXd actual;
        EXPECT_THROW(select_rows(in, rows, actual), eigen_utilities::assert_error);
        EXPECT_NO_THROW(select_rows_resize(in, rows, actual));
        EXPECT_EQ(expected_rows, actual);
    }
    
    
    Eigen::VectorXi cols(2);
    cols << 1, 0;
    Eigen::MatrixXd expected_cols(in.rows(), cols.size());
    expected_cols <<
        10, 0,
        40, 30,
        70, 60;
    
    {
        Eigen::MatrixXd actual(expected_cols.rows(), expected_cols.cols());
        actual.setZero();
        select_cols(in, cols, actual);
        EXPECT_EQ(expected_cols, actual);
    }
    {
        // Test resizing
        Eigen::MatrixXd actual;
        EXPECT_THROW(select_cols(in, cols, actual), eigen_utilities::assert_error);
        EXPECT_NO_THROW(select_cols_resize(in, cols, actual));
        EXPECT_EQ(expected_cols, actual);
    }
    
    Eigen::MatrixXd expected_grid(rows.size(), cols.size());
    expected_grid <<
        10, 0,
        70, 60;
    
    {
        Eigen::MatrixXd actual(expected_grid.rows(), expected_grid.cols());
        actual.setZero();
        select_grid(in, rows, cols, actual);
        EXPECT_EQ(expected_grid, actual);
    }
    {
        // Test resizing
        Eigen::MatrixXd actual;
        EXPECT_THROW(select_grid(in, rows, cols, actual), eigen_utilities::assert_error);
        EXPECT_NO_THROW(select_grid_resize(in, rows, cols, actual));
        EXPECT_EQ(expected_grid, actual);
    }
}
