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

inline void yaml_write_binary(YAML::Emitter &out, const Eigen::VectorXd &X)
{
    out << YAML::Flow << YAML::BeginSeq;
    for (int i = 0; i < X.rows(); ++i)
        yaml_write_binary(out, X.coeff(i));
    out << YAML::EndSeq;
}

inline void yaml_read_binary(const YAML::Node &out, Eigen::VectorXd &X)
{
    int rows = out.size();
    X.resize(rows);
    for (int i = 0; i < rows; ++i)
        yaml_read_binary(out[i], X(i));
}

/// @note Copy of template, but specializing. Must do this to ensure that Eigen::VectorXd is called
inline void yaml_write_binary_dual(YAML::Emitter &out, const Eigen::VectorXd &X)
{
    // Write a tuple of dual-representation
    out << YAML::BeginMap
        << YAML::Key << "value" << YAML::Value << X
        << YAML::Key << "base64" << YAML::Value;
    yaml_write_binary(out, X);
    out << YAML::EndMap;
}

inline void yaml_read_binary_dual(const YAML::Node &node, Eigen::VectorXd &X)
{
    // Read the binary base64 portion
    yaml_read_binary(node["base64"], X);
}

}

#endif
