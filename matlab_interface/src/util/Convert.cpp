#include "amber/matlab/util/Convert.hpp"

using namespace amber::easy;

namespace amber
{
namespace matlab
{

std::streamsize StdOutBuffer::xsputn(const char *s, std::streamsize n)
{
    mexPrintf("%.*s",n,s);
    return n;
}

int StdOutBuffer::overflow(int c)
{
    if (c != EOF)
    {
        mexPrintf("%.1s",&c);
    }
    return 1;
}

// Modified From: http://svn.anu.edu.au/AI/drwn/trunk/projects/matlab/drwnMatlabUtils.h

void mxArrayToEigen(const mxArray *m, MatrixXd &A)
{
    const int nRows = mxGetM(m); // number of rows
    const int nCols = mxGetN(m); // number of columns
    // MX_DEBUG("parsing a %d-by-%d matrix...", nRows, nCols);

    A.resize(nRows, nCols);
    const double *p = mxGetPr(m);
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            A(i, j) = p[i + j * nRows];
        }
    }
}

void mxArrayToEigen(const mxArray *m, VectorXd &b)
{
    // This won't handle bad arrays well, but oh well
    const int nRows = mxGetM(m); // number of rows
    const int nCols = mxGetN(m); // number of columns

    b = Eigen::Map<VectorXd>(mxGetPr(m), nRows * nCols);
}

mxArray *mxEigenToArray(const MatrixXd &A)
{
    const int nRows = A.rows(), nCols = A.cols();
    mxArray *m = mxCreateDoubleMatrix(nRows, nCols, mxREAL);
    double *p = mxGetPr(m);
    for (int i = 0; i < nRows; i++)
        for (int j = 0; j < nCols; j++)
            p[i + j * nRows] = A(i, j);
    return m;
}

mxArray *mxEigenToArray(const VectorXd &b)
{
    const int nRows = b.rows();
    mxArray *m = mxCreateDoubleMatrix(nRows, 1, mxREAL);
    double *p = mxGetPr(m);
    for (int i = 0; i < nRows; i++)
        p[i] = b(i);
    return m;
}

void mxArrayToString(const mxArray *in, string &out)
{
    char *raw = mxArrayToString(in);
    out = raw;
    mxFree(raw);
}

mxArray *mxStringToArray(const string &in)
{
    return mxCreateString(in.c_str());
}

mxArray *to_mex(const string &str)
{
    return mxStringToArray(str);
}

void from_mex(const mxArray *m, std::string &str)
{
    mxArrayToString(m, str);
}

mxArray *to_mex(double value)
{
    return mxCreateDoubleScalar(value);
}

void from_mex(const mxArray *m, double &value)
{
    value = mxGetScalar(m);
}

mxArray *to_mex(bool value)
{
    return mxCreateLogicalScalar(value);
}

void from_mex(const mxArray *m, bool &value)
{
    // Meh
    value = mxGetScalar(m);
}

mxArray *to_mex(int value)
{
    return to_mex((double)value);
}

void from_mex(const mxArray *m, int &value)
{
    value = mxGetScalar(m);
}

}
}
