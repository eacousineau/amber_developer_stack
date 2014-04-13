/*
 * MatlabInterface.h
 *
 *  Created on: Aug 26, 2012
 *      Author: eacousineau
 */

#ifndef MATLABINTERFACEBASE_H_
#define MATLABINTERFACEBASE_H_

#include <amber/matlab/util.hpp>

#define AMBER_MATLAB_MAIN(interface) \
	void mexFunction(int nargout, mxArray *argout[], int nargin, const mxArray *argin[]) \
	{ \
		(interface).main(amber::matlab::Args(nargout, argout, nargin, argin)); \
	}

namespace amber
{
namespace matlab
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
} /* namespace AmberSim */
#endif /* MATLABINTERFACEBASE_H_ */
