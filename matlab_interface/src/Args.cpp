#include <matlab_interface/Args.hpp>

namespace matlab_interace
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
    assert_ex_msg(nin >= 1, "Not enough arguments");
    --nin;
    const mxArray *arg = in[0];
    ++in;
    return arg;
}

void Args::push(mxArray *aout)
{
    assert_ex_msg(nout >= 1, "Not enough output arguments");
    --nout;
    *out = aout;
    ++out;
}

Args Args::shiftCopy()
{
    assert_ex_msg(nin >= 1, "Not enough arguments");
    return Args(nout, out, nin - 1, in - 1);
}

}
