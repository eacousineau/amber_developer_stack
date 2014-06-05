#include "eigen_utilities/runtime_config.hpp"

#include <Eigen/Dense>

/// @todo How does this handle compilation flags? Does ::set_is_malloc_allowed() disappear with DNDEBUG?

namespace eigen_utilities
{

static int disable_malloc_counter = 0;

int disable_malloc()
{
    if (disable_malloc_counter == 0)
        Eigen::internal::set_is_malloc_allowed(false);
    return ++disable_malloc_counter;
}

int enable_malloc()
{
    common_assert(disable_malloc_counter > 0);
    --disable_malloc_counter;
    if (disable_malloc_counter == 0)
        Eigen::internal::set_is_malloc_allowed(true);
    return disable_malloc_counter;
}

bool is_malloc_enabled()
{
    return disable_malloc_counter == 0;
}

}
