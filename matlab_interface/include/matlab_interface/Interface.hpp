/*
 * MatlabInterface.h
 *
 *  Created on: Aug 26, 2012
 *      Author: eacousineau
 */

#ifndef MATLABINTERFACEBASE_H_
#define MATLABINTERFACEBASE_H_

#include <boost/shared_ptr.hpp>
#include <matlab_interface/util.hpp>

#define MATLAB_INTERFACE_MAIN(interface) \
	void mexFunction(int nargout, mxArray *argout[], int nargin, const mxArray *argin[]) \
	{ \
		(interface).main(matlab_interface::Args(nargout, argout, nargin, argin)); \
	}

namespace matlab_interface
{

/**
 * \brief Interface for talking with Matlab through the MEX library.
 */
class Interface
{
private:
    StdOutBuffer matlabBuf;
    std::streambuf *stdoutBuf;
    bool isLoaded;
protected:
    CommandList commands;
public:
    Interface();
    virtual ~Interface();

    /**
     * \brief The simplified entry point for the interface.
     */
    virtual void main(Args args);
    virtual void init(Args args);
    virtual void load() = 0;

    static boost::shared_ptr<Interface> instance;
};

}

#endif /* MATLABINTERFACEBASE_H_ */
