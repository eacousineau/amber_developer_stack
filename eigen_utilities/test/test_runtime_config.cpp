#include <gtest/gtest.h>

#undef NDEBUG
#include <eigen_utilities/runtime_config.hpp>

#include <Eigen/Dense>

TEST(eigen_utilities, check_disabled)
{
    int counter;
    counter = eigen_utilities::disable_malloc();
    EXPECT_EQ(1, counter);
    EXPECT_FALSE(eigen_utilities::is_malloc_enabled());
    {
        eigen_utilities::DisableMallocScope scope;
        EXPECT_EQ(2, scope.getCounter());
        EXPECT_FALSE(eigen_utilities::is_malloc_enabled());
        EXPECT_FALSE(Eigen::internal::is_malloc_allowed());
    }
    counter = eigen_utilities::enable_malloc();
    EXPECT_EQ(0, counter);
    EXPECT_TRUE(eigen_utilities::is_malloc_enabled());
}

TEST(eigen_utilities, check_bad_disable)
{
    EXPECT_THROW(eigen_utilities::enable_malloc(), common::assert_error);
}

TEST(eigen_utilities, check_alloc)
{
    Eigen::MatrixXd blank;
    // Expect no error
    {
        EXPECT_NO_THROW(blank.resize(2, 2));
    }
    
    // Now expect it to error out
    {
        eigen_utilities::DisableMallocScope scope;
        EXPECT_THROW(blank.resize(4, 4), common::assert_error);
    }
    
}

TEST(eigen_utilities, check_nan)
{
    Eigen::MatrixXd blank(3, 3);
    
    // Ensure that all are initialized to nan
    EXPECT_TRUE((blank.array() != blank.array()).all());
    std::cout << blank << std::endl;
    
    blank.resize(5, 5);
    EXPECT_TRUE((blank.array() != blank.array()).all());
    std::cout << blank << std::endl;
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
