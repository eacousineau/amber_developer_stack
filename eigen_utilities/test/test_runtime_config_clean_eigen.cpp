#include <Eigen/Dense>

// See test_runtime_config for more information

namespace test_runtime_config_clean_eigen
{

void resize_matrix()
{
    Eigen::MatrixXd blank;
    blank.resize(5, 5);
}

}
