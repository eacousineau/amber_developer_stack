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

// All entries of newly constructed matrices and arrays are initialized to NaN, as are new 
// entries in matrices and arrays after resizing.
#define EIGEN_INITIALIZE_MATRICES_BY_NAN

// Adds a switch which can be turned on and off by calling set_is_malloc_allowed(bool). 
// If malloc is not allowed and Eigen tries to allocate memory dynamically anyway, an assertion 
// failure results.
// In this case, a common_assert::assert_error will be thrown
#define EIGEN_RUNTIME_NO_MALLOC

// Switch eigen_assert to throw an exception
#ifdef eigen_assert
    #undef eigen_assert
#endif
#define eigen_assert(x)  common_assert(x)

namespace eigen_utilities
{

/**
 * @brief disable_malloc Increment a global reference counter. If nonzero, it will disable Eigen malloc() calls
 * @note NOT THREAD SAFE! Uses Eigen::internal::set_is_malloc_allowed()
 * @return Reference counter value
 */
int disable_malloc();

/**
 * @brief enable_malloc Decrement a global reference counter. If zero, it will enable Eigen malloc() calls
 * @note NOT THREAD SAFE! Throws a std::runtime_error if the reference counter is decremented below zero
 * @return Reference counter value
 */
int enable_malloc();

/**
 * @brief is_malloc_enabled Return if malloc() is enabled for Eigen
 * @return 
 */
bool is_malloc_enabled();

/**
 * @brief Disable malloc() within a certain scope using reference countin
 */
class DisableMallocScope
{
    int counter;
public:
    inline DisableMallocScope()
    {
        counter = disable_malloc();
    }
    
    inline ~DisableMallocScope()
    {
        enable_malloc();
    }
    
    /** @brief Get the counter for when this object was constructed */
    inline int getCounter() const
    {
        return counter;
    }
};

}

#endif // EIGEN_UTILITIES_RUNTIME_CONFIG_H_
