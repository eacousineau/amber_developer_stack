#include <common_assert/common_assert.hpp>

#include <matlab_interface/Args.hpp>

namespace matlab_interface
{

Args::Args()
    : nout(0), nin(0), out(NULL), in(NULL)
{ }

Args::Args(int nargout, mxArray *argout[], int nargin, const mxArray *argin[])
    : nout(nargout), nin(nargin), out(argout), in(argin)
{}

// Shift input arguments
const mxArray *Args::shift()
{
    common_assert_msg(nin >= 1, "Not enough arguments");
    --nin;
    const mxArray *arg = in[0];
    ++in;
    return arg;
}

void Args::push(mxArray *aout)
{
    common_assert_msg(nout >= 1, "Not enough output arguments");
    --nout;
    *out = aout;
    ++out;
}

Args Args::shiftCopy()
{
    common_assert_msg(nin >= 1, "Not enough arguments");
    return Args(nout, out, nin - 1, in - 1);
}

}
