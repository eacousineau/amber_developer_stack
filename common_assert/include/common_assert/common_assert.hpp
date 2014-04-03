/**
 * @brief Utility functions for assertions by throwing informative exceptions
 * @author Eric Cousineau <eacousineau@gmail.com>
 */
#ifndef _COMMON_ASSERT_
#define _COMMON_ASSERT_

#include <sstream>
#include <stdexcept>

namespace common
{

class assert_error : public std::runtime_error
{
public:
    inline assert_error(const std::string &message)
        : std::runtime_error(message)
    { }
};

}

/// @todo Place in another package
#define common_assert_msg(expr, msg) \
    if (!(expr)) \
    { \
        std::ostringstream os; \
        os << "Assert Error: " << #expr << std::endl << \
            "File: " << __FILE__ ":" << __LINE__ << std::endl << \
            "Function: " << __PRETTY_FUNCTION__ << std::endl << \
            "Message: " << msg << std::endl; \
        throw common::assert_error(os.str()); \
    }

#define common_assert(expr) \
    common_assert_msg(expr, "[None]")

#endif // _COMMON_ASSERT_
