#ifndef MATLAB_INTERFACE_CONVERT_H
#define MATLAB_INTERFACE_CONVERT_H

#include <mex.h>
#include <matrix.h>

#include <Eigen/Dense>
#include <string>

// TODO Add in auto to_mex stuff for vector (cell array), maybe maps
//TODO Make sure these things are type-safe. Look into USC guy's C++ Matlab wrappers.

namespace matlab_interface
{

// From: http://stackoverflow.com/questions/243696/correctly-over-loading-a-stringbuf-to-replace-cout-in-a-matlab-mex-file
class StdOutBuffer : public std::streambuf
{
protected:
    virtual std::streamsize xsputn(const char *s, std::streamsize n);
    virtual int overflow(int c = EOF);
};

// Change this to some sort of convert<T> setup - in-place conversion?

void mxArrayToEigen(const mxArray *m, Eigen::MatrixXd &A);
void mxArrayToEigen(const mxArray *m, Eigen::VectorXd &b);

template<int rows, int cols>
void mxArrayToEigen(const mxArray *m, Eigen::Matrix<double, rows, cols> &A)
{
    Eigen::MatrixXd Ax;
    mxArrayToEigen(m, Ax);
    A = Ax;
}

mxArray *mxEigenToArray(const Eigen::MatrixXd &A);

mxArray *mxEigenToArray(const Eigen::VectorXd &b);

template<int rows, int cols>
mxArray *mxEigenToArray(const Eigen::Matrix<double, rows, cols>& A)
{
    return mxEigenToArray(Eigen::MatrixXd(A));
}

void mxArrayToString(const mxArray *in, std::string &out);
mxArray *mxStringToArray(const std::string &in);

mxArray *to_mex(const std::string &str);
inline mxArray *to_mex(const char *str)
{
    return to_mex(std::string(str));
}
void from_mex(const mxArray *m, std::string &str);

mxArray *to_mex(double value);
void from_mex(const mxArray *m, double &value);

mxArray *to_mex(bool value);
void from_mex(const mxArray *m, bool &value);

mxArray *to_mex(int value);
void from_mex(const mxArray *m, int &value);

inline mxArray *to_mex(const Eigen::VectorXd &X)
{
    return mxEigenToArray(X);
}
inline void from_mex(const mxArray *m, Eigen::VectorXd &X)
{
    mxArrayToEigen(m, X);
}

inline mxArray *to_mex(mxArray *m)
{
    return m;
}

inline mxArray *to_mex(const Eigen::MatrixXd &X)
{
    return mxEigenToArray(X);
}
inline void from_mex(const mxArray *m, Eigen::MatrixXd &X)
{
    mxArrayToEigen(m, X);
}

template<int rows, int cols>
mxArray *to_mex(const Eigen::Matrix<double, rows, cols>& X)
{
    return mxEigenToArray(Eigen::MatrixXd(X));
}


template<int rows, int cols>
void from_mex(const mxArray *m, Eigen::Matrix<double, rows, cols>& X)
{
    return mxArrayToEigen(m, Eigen::MatrixXd(X));
}

}

#endif // MEX_UTIL_H
