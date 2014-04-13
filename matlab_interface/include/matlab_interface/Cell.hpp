/*
 * Struct.h
 *
 *  Created on: Feb 6, 2013
 *      Author: eacousineau
 */

#ifndef MATLAB_INTERFACE_CELL_H_
#define MATLAB_INTERFACE_CELL_H_

#include <vector>
#include <matlab_interface/conversion.hpp>

namespace matlab_interface
{

// For now can only write
class Cell
{
    typedef std::vector<mxArray *> Values;
    Values values;
public:
    Cell();

    int add(mxArray *value);

    template<typename T>
    int add(const T &value)
    {
        return add(to_mex(value));
    }

    mxArray *as_mex();
};

inline mxArray *to_mex(Cell &obj)
{
    return obj.as_mex();
}

template<typename T>
inline Cell &operator<<(Cell &cell, const T &value)
{
    cell.add(value);
    return cell;
}

}

#endif /* STRUCT_H_ */
