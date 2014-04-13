#ifndef MATLAB_INTERFACE_CONVERT_H
#define MATLAB_INTERFACE_CONVERT_H

#include <mex.h>
#include <matrix.h>

#include <amber/util.hpp>

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

void mxArrayToEigen(const mxArray *m, MatrixXd &A);
void mxArrayToEigen(const mxArray *m, VectorXd &b);

template<int rows, int cols>
void mxArrayToEigen(const mxArray *m, Eigen::Matrix<double, rows, cols> &A)
{
    MatrixXd Ax;
    mxArrayToEigen(m, Ax);
    A = Ax;
}

mxArray *mxEigenToArray(const MatrixXd &A);

mxArray *mxEigenToArray(const VectorXd &b);

template<int rows, int cols>
mxArray *mxEigenToArray(const Eigen::Matrix<double, rows, cols>& A)
{
    return mxEigenToArray(MatrixXd(A));
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

inline mxArray *to_mex(const VectorXd &X)
{
    return mxEigenToArray(X);
}
inline void from_mex(const mxArray *m, VectorXd &X)
{
    mxArrayToEigen(m, X);
}

inline mxArray *to_mex(mxArray *m)
{
    return m;
}

inline mxArray *to_mex(const MatrixXd &X)
{
    return mxEigenToArray(X);
}
inline void from_mex(const mxArray *m, MatrixXd &X)
{
    mxArrayToEigen(m, X);
}

template<int rows, int cols>
mxArray *to_mex(const Eigen::Matrix<double, rows, cols>& X)
{
    return mxEigenToArray(MatrixXd(X));
}


template<int rows, int cols>
void from_mex(const mxArray *m, Eigen::Matrix<double, rows, cols>& X)
{
    return mxArrayToEigen(m, MatrixXd(X));
}

}

#endif // MEX_UTIL_H
