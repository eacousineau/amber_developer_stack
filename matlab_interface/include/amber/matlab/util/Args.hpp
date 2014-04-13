#ifndef AMBER_MATLAB_UTIL_H
#define AMBER_MATLAB_UTIL_H

#include <mex.h>
#include <matrix.h>

#include <amber/util.hpp>
#include <amber/matlab/util/Convert.hpp>

// http://www.mathworks.com/help/matlab/apiref/
// http://www.mathworks.com/help/matlab/using-mxarrays-for-writing-c-c-and-fortran-programs-for-use-with-matlab.html
// http://www.mathworks.com/help/matlab/matlab_external/examples-of-c-c-source-mex-files.html#bq9posx-1

namespace amber
{
namespace matlab
{

// Todo: Split into In and Out
struct Args
{
    int nout, nin;
    mxArray **out;
    const mxArray **in;
    // Keep track of originals? Have reset() func?

    Args();
    Args(int nargout, mxArray *argout[], int nargin, const mxArray *argin[]);

    // Shift input arguments
    const mxArray *shift();
    void push(mxArray *aout); // Will this be too confusing?
    Args shiftCopy();
};

template<typename T>
Args &operator<<(Args &args, const T &value)
{
    args.push(to_mex(value));
    return args;
}

template<typename T>
Args &operator>>(Args &args, T &value)
{
    from_mex(args.shift(), value);
    return args;
}

}
}

#endif // MEX_UTIL_H
