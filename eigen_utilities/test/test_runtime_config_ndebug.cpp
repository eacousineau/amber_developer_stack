#ifndef NDEBUG
    #define NDEBUG
#endif

#include <eigen_utilities/runtime_config.hpp>
#include <Eigen/Dense>

// See test_runtime_config for more information

namespace test_runtime_config_ndebug
{

int resize_matrix_calling_disable()
{
    eigen_utilities::DisableMallocScope scope;
    Eigen::MatrixXd blank;
    blank.resize(5, 5);
    return scope.getCounter();
}
    
}
