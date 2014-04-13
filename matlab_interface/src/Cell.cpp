/*
 * Struct.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: eacousineau
 */

#include <matlab_interface/Cell.hpp>

namespace matlab_interface
{

Cell::Cell()
{ }

int Cell::add(mxArray *value)
{
    values.push_back(value);
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
