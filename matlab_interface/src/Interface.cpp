/*
 * MatlabInterface.cpp
 *
 *  Created on: Aug 26, 2012
 *      Author: eacousineau
 */

#include <iostream>

#include "matlab_interface/Interface.hpp"

using namespace std;

namespace matlab_interface
{

boost::shared_ptr<Interface> Interface::instance;

Interface::Interface()
    : isLoaded(false),
      commands("amber_sim", "Amber Model Sim C++ Module Thing")
{
    // What about error?
    stdoutBuf = std::cout.rdbuf(&matlabBuf);
    //mexAtExit(&matlab_unload); // May not be needed because of deconstructor
    cout << "Loaded amber_matlab C++ module\n";

    commands.add(new Command("init", "Load up stuff", "",
            boost::bind(&Interface::init, this, _1)));
}

Interface::~Interface()
{
    cout << "Unloaded amber_matlab C++ module\n";
}

void Interface::main(Args args)
{
    try
    {
        bool isExplicitInit = false;
        if (args.nin > 0)
        {
            // Peek to see if it's init, then head it off
            string cmd;
            from_mex(args.in[0], cmd);
            if (cmd == "init")
                isExplicitInit = true;
        }

        // This doesn't play well with Matlab...
        if (!isExplicitInit && !isLoaded)
            init(Args());

        commands.dispatch(args);
    }
    catch (std::exception &e)
    {
        std::ostringstream os;
        os << "[ C++ Exception ]\n" << e.what();
        mexErrMsgTxt(os.str().c_str());
    }
}

void Interface::init(Args args)
{
    load();
    isLoaded = true;
}

}
