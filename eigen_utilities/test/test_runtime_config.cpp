#include <gtest/gtest.h>

#include <eigen_utilities/runtime_config.hpp>

#include <Eigen/Dense>

/// @todo Mix source files with and without NDEBUG and runtime_config
/// Mix the function calls

TEST(eigen_utilities, check_disabled)
{
    EXPECT_TRUE(Eigen::internal::is_malloc_allowed());
    
    {
        eigen_utilities::DisableMallocScope scope;
        EXPECT_TRUE(scope.wasMallocPreviouslyEnabled());
        EXPECT_FALSE(Eigen::internal::is_malloc_allowed());
        
        {
            eigen_utilities::DisableMallocScope scope;
            EXPECT_FALSE(scope.wasMallocPreviouslyEnabled());
            EXPECT_FALSE(Eigen::internal::is_malloc_allowed());
        }
        
        EXPECT_FALSE(Eigen::internal::is_malloc_allowed());
    }
    
    EXPECT_TRUE(Eigen::internal::is_malloc_allowed());
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
        , eigen_utilities::assert_error);
        
        // Check nesting once more
        {
            eigen_utilities::DisableMallocScope scope;
            EXPECT_THROW(
                Eigen::VectorXd vec(5);
            , eigen_utilities::assert_error);
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

/// @warning When using different compiler optimization flags (namely debug), inlined functions may be actually not be inlined
/// For this reason, these 'extern' checks are disabled

#ifdef EIGEN_UTILITIES_CHECK_EXTERN
// Forward declare stuff from other source files with different preprocessor values

namespace test_runtime_config_ndebug
{
    /**
     * @brief check_alloc Allocate a dynamically-sized matrix in a source file NDEBUG defined (where malloc() switches should have no effect)
     * @return Return if malloc was previously disabled
     */
    bool resize_matrix_calling_disable();
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

TEST(eigen_utilities, check_alloc_extern)
{
    // Check alloc when including from source files without the MALLOC definition defined
    {
        eigen_utilities::DisableMallocScope scope;
        
        // Check for source file with NDEBUG defined
        int old_value = true;
        EXPECT_NO_THROW(
            old_value = test_runtime_config_ndebug::resize_matrix_calling_disable();
        );
        EXPECT_FALSE(old_value);
        
        // Check for source file with only Eigen included
        EXPECT_NO_THROW(
            test_runtime_config_clean_eigen::resize_matrix();
        );
        
        // Double-check for functions included in this file
        EXPECT_THROW(
            resize_matrix();
        , eigen_utilities::assert_error);
    }
}
#endif

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
