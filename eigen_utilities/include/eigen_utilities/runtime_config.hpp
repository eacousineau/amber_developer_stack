/**
  * @brief Define additional macros to change the runtime behavior of eigen
  * Namely this initializes matrices by NAN, adds the ability to disable runtime allocation, and
  * provide mechanisms to enable and disable this runtime allocation (for better assurance in real-time
  * programming
  * @note To use, include this before including any Eigen/ files
  * THIS WILL HAVE NO EFFECT OTHERWISE!
  * @author J. D. Yamakoski
  * @author Eric Cousineau
  * @ref http://eigen.tuxfamily.org/dox-devel/TopicPreprocessorDirectives.html
  * @ref http://www.orocos.org/forum/rtt/rtt-dev/nice-trick-those-using-eigen-realtime-code
  */
#ifndef EIGEN_UTILITIES_RUNTIME_CONFIG_H_
    #define EIGEN_UTILITIES_RUNTIME_CONFIG_H_

#include <common_assert/common_assert.hpp>

// Use EIGEN_UTILITIES_DEBUG instead of !NDEBUG for more flexibility
#ifdef EIGEN_UTILITIES_DEBUG

    /// @note For my version of eigen3, `dpkg -s libeigen3-dev` on Ubuntu 12.04 yields "3.0.5-1",
    /// there are no include or #ifdef guards for EIGEN_INITIALIZE_MATRICES_BY_NAN, only
    /// EIGEN_INITIALIZE_MATRICES_BY_ZERO... When was this introduced?

    //// All entries of newly constructed matrices and arrays are initialized to NaN, as are new 
    //// entries in matrices and arrays after resizing.
    //#define EIGEN_INITIALIZE_MATRICES_BY_NAN
    #define EIGEN_INITIALIZE_MATRICES_BY_ZERO
    
    // Adds a switch which can be turned on and off by calling set_is_malloc_allowed(bool). 
    // If malloc is not allowed and Eigen tries to allocate memory dynamically anyway, an assertion 
    // failure results.
    // In this case, a eigen_utilities::assert_error will be thrown
    #define EIGEN_RUNTIME_NO_MALLOC

    // Forward declare from Eigen[v3.0.5]/Core/util/Memory.h:193
    namespace Eigen
    {
        namespace internal
        {
            inline bool is_malloc_allowed();
            inline bool set_is_malloc_allowed(bool new_value);
        }
    }
    
    // Switch eigen_assert to throw an exception
    #ifdef eigen_assert
        #undef eigen_assert
    #endif
    #define eigen_assert(x)  common_assert_msg_ex(x, "[None]", eigen_utilities::assert_error)

#endif

namespace eigen_utilities
{

/**
 * @brief Specialized assert_error
 */
class assert_error : public common::assert_error
{
public:
    inline assert_error(const std::string &message)
        : common::assert_error(message)
    { }
};

/**
 * @brief Disable malloc() within a certain scope by storing previous value
 */
class DisableMallocScope
{
    bool old_value;
public:
    #ifdef EIGEN_RUNTIME_NO_MALLOC
        inline DisableMallocScope()
        {
            old_value = Eigen::internal::is_malloc_allowed();
            Eigen::internal::set_is_malloc_allowed(false);
        }
        
        inline ~DisableMallocScope()
        {
            Eigen::internal::set_is_malloc_allowed(old_value);
        }
    #else
        inline DisableMallocScope()
        {
            old_value = true;
            // Do nothing otherwise
        }
        
        inline ~DisableMallocScope()
        { }
    #endif
    
    /** @brief Get the counter for when this object was constructed */
    inline bool wasMallocPreviouslyEnabled() const
    {
        return old_value;
    }
};

}

#endif // EIGEN_UTILITIES_RUNTIME_CONFIG_H_
