#include <gtest/gtest.h>

#ifdef NDEBUG
    #undef NDEBUG
#endif
#include <eigen_utilities/runtime_config.hpp>

#include <Eigen/Dense>

// Forward declare stuff from other source files with different sybmols
namespace test_runtime_config_ndebug
{
    /**
     * @brief check_alloc Allocate a dynamically-sized matrix in a source file NDEBUG defined (where malloc() switches should have no effect)
     * @return Return the reference counter when eigen_utilites::enable_malloc() is called (should always be zero)
     */
    int resize_matrix_calling_disable();
}

namespace test_runtime_config_clean_eigen
{
    /**
     * @brief check_alloc Allocate a dynamically-sized matrix in a source file with a clean include of Eigen
     */
    void resize_matrix();
}

int resize_matrix()
{
    eigen_utilities::DisableMallocScope scope;
    Eigen::MatrixXd blank;
    blank.resize(5, 5);
}

/// @todo Mix source files with and without NDEBUG and runtime_config
/// Mix the function calls

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
    /// @note When the exception is thrown during a resize, it dies on the deconstructor
    /// if the variable is scoped outside of the resize() statement. Most likely due to the
    /// catch() for gtest EXPECT_THROW()
    
    // Expect no error
    {
        EXPECT_NO_THROW(
            Eigen::MatrixXd blank;
            blank.resize(2, 2)
        );
    }
    
    // Now expect it to error out
    {
        eigen_utilities::DisableMallocScope scope;
        EXPECT_THROW(
            Eigen::MatrixXd blank;
            blank.resize(4, 4);
        , common::assert_error);
        
        // Check nesting once more
        {
            eigen_utilities::DisableMallocScope scope;
            EXPECT_THROW(
                Eigen::VectorXd vec(5);
            , common::assert_error);
        }
    }
    
    // Check once again, ensuring that it is still disabled
    {
        EXPECT_NO_THROW(
            Eigen::MatrixXd blank;
            blank.resize(10, 10)
        );
    }
}

TEST(eigen_utilities, check_alloc_extern)
{
    // Check alloc when including from source files without the MALLOC definition defined
    {
        eigen_utilities::DisableMallocScope scope;
        
        // Check for source file with NDEBUG defined
        int counter = -1;
        EXPECT_NO_THROW(
            counter = test_runtime_config_ndebug::resize_matrix_calling_disable();
        );
        EXPECT_EQ(0, counter);
        
        // Check for source file with only Eigen included
        EXPECT_NO_THROW(
            test_runtime_config_clean_eigen::resize_matrix();
        );
        
        // Double-check for functions included in this file
        EXPECT_THROW(
            resize_matrix();
        , common::assert_error);
    }
}

TEST(eigen_utilities, check_zero)
{
    Eigen::MatrixXd blank(3, 3);
    
    // Ensure that all are initialized to zero
    EXPECT_TRUE((blank.array() == 0).all());
    
    blank.resize(5, 5);
    EXPECT_TRUE((blank.array() == 0).all());
}

//TEST(eigen_utilities, check_nan)
//{
//    Eigen::MatrixXd blank(3, 3);
    
//    // Ensure that all are initialized to nan
//    EXPECT_TRUE((blank.array() != blank.array()).all());
//    std::cout << blank << std::endl;
    
//    blank.resize(5, 5);
//    EXPECT_TRUE((blank.array() != blank.array()).all());
//    std::cout << blank << std::endl;
//}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
