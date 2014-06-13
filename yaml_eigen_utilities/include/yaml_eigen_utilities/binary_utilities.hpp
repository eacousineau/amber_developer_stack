/**
  * @brief Quick hack to get binary values
  * @warning MACHINE DEPENDENT!
  */
#ifndef _YAML_EIGEN_UTILITIES_BINARY_UTILITIES_HPP_
    #define _YAML_EIGEN_UTILITIES_BINARY_UTILITIES_HPP_

#include <yaml_utilities/binary_utilities.hpp>
#include <yaml_eigen_utilities/yaml_eigen_utilities.hpp>

// Note the namespacing
namespace yaml_utilities
{

    template<>
inline void yaml_write_binary(YAML::Emitter &out, const Eigen::VectorXd &X)
{
    out << YAML::Flow << YAML::BeginSeq;
    for (int i = 0; i < X.rows(); ++i)
        yaml_write_binary(out, X.coeff(i));
    out << YAML::EndSeq;
}

    template<>
inline void yaml_read_binary(const YAML::Node &out, Eigen::VectorXd &X)
{
    int rows = out.size();
    X.resize(rows);
    for (int i = 0; i < rows; ++i)
        yaml_read_binary(out[i], X(i));
}

}

#endif
