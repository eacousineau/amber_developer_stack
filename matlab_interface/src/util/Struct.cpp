/*
 * Struct.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: eacousineau
 */

#include <matlab_interface/util/Struct.hpp>

namespace matlab_interface
{

Struct::Struct()
{
    data = mxCreateStructMatrix(1, 1, 0, NULL);
}

int Struct::set(const string &name, mxArray *value)
{
    FieldMap::iterator iter = fieldMap.find(name);
    int index;
    if (iter == fieldMap.end())
    {
        index = mxAddField(data, name.c_str());
        fieldMap[name] = index;
    }
    else
        index = iter->second;
    mxSetFieldByNumber(data, 0, index, value);
    return index;
}

}
