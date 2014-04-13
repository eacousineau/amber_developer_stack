/*
 * Struct.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: eacousineau
 */

#include <amber/matlab/util/Cell.hpp>

using namespace amber::easy;

namespace amber
{
namespace matlab
{

Cell::Cell()
{ }

int Cell::add(mxArray *value)
{
    values << value;
    return values.size() - 1;
}

mxArray *Cell::as_mex()
{
    uint count = values.size();
    mxArray *data = mxCreateCellMatrix(count, 1);
    for (uint i = 0; i < count; ++i)
        mxSetCell(data, i, values[i]);
    return data;
}

}
}
