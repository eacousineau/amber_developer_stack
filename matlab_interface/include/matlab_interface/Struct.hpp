/*
 * Struct.h
 *
 *  Created on: Feb 6, 2013
 *      Author: eacousineau
 */

#ifndef MATLAB_INTERFACE_STRUCT_H_
#define MATLAB_INTERFACE_STRUCT_H_

#include <map>
#include <matlab_interface/Convert.hpp>

namespace matlab_interface
{

// For now can only write
class Struct
{
    typedef std::map<std::string, int> FieldMap;
    FieldMap fieldMap;
public:
    mxArray *data;

    Struct();

    int set(const std::string &field, mxArray *value);

    template<typename T>
    int set(const std::string &field, const T &value)
    {
        return set(field, to_mex(value));
    }
};

inline mxArray *to_mex(Struct &obj)
{
    return obj.data;
}

}

#endif /* STRUCT_H_ */
