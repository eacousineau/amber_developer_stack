// Disable debugging in this translation unit
#ifdef EIGEN_UTILITIES_DEBUG
    #undef EIGEN_UTILITIES_DEBUG
#endif

#include <eigen_utilities/runtime_config.hpp>
#include <Eigen/Dense>

// See test_runtime_config for more information

namespace test_runtime_config_ndebug
{

bool resize_matrix_calling_disable()
{
    eigen_utilities::DisableMallocScope scope;
    Eigen::MatrixXd blank;
    blank.resize(5, 5);
    return scope.wasMallocPreviouslyEnabled();
}
    
}
