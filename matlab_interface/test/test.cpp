#include <common_assert/common_assert.hpp>

#include "matlab_interface/Interface.hpp"

using namespace matlab_interface;
using namespace Eigen;
using namespace std;

/**
 * @todo I want to pass an entry point in matlab for it to call...
 * http://dirkgorissen.com/2012/02/06/bridging-python-and-matlab-passing-callbacks-and-optimization/
 * Looking at: https://github.com/dgorissen/pymatopt.git
 */

/**
 * @brief TestInterface A test to show some of the capabilities of matlab_interface::Interface
 */
class TestInterface : public Interface
{
    /** @brief A counter to show persistence */
    int counter;
public:
    TestInterface()
        : Interface("matlab_interface_test", "Test interface"),
        counter(0)
    {        
        commands.add(new Command("variable_arguments", "Print number of inputs and outputs, and "
                "assign i = 1..nargout to varargout", "[varargout] = f(varargin)",
                boost::bind(&TestInterface::variableArguments, this, _1)));
        
        commands.add(new Command("reset_counter", "Reset counter", "[count] = f()",
                boost::bind(&TestInterface::resetCounter, this, _1)));
        commands.add(new Command("increment_counter", "Increment to show persistence", "",
                boost::bind(&TestInterface::incrementCounter, this, _1)));
        
        commands.add(new Command("generate_struct",
                "Generate a structure, printing results in C++, with name as string, A as matrix, b as scalar:\n"
                "\tobj = struct('name', name, ...\n"
                "\t\t'cell', {A, b, 'Hello'});\n"
                "\tvalue = -b * A';",
                "[obj, value] = f(name, A, b)",
                boost::bind(&TestInterface::generateStruct, this, _1)));
    }

    void load()
    {
    }

    void generateStruct(Args args)
    {
        common_assert_msg(args.nin == 3 && args.nout == 2,
            "Need 3 inputs (given " << args.nin << ") and 2 outputs (given " << args.nout << ")");

        // Get inputs
        string name;
        MatrixXd A;
        double b;
        args >> name >> A >> b;

        MatrixXd value = -b * A.transpose();
        cout << "A: " << A
            << "\nb: " << b
            << "\nvalue: " << value << "\n";

        Struct obj;
        Cell cell;
        /// @todo Throw error if b is not a scalar!
        cell << A << b << "Hello";
        obj.set("name", "Test");
        obj.set("cell", to_mex(cell));

        // TODO Make struct stuff more elegant with const' stuff later
        args.push(to_mex(obj));
        args << value;
    }

    void variableArguments(Args args)
    {
        cout << "Given " << args.nin << " input and " << args.nout << " output arguments.\n";
        
        // Assign arguments so that Matlab does not complain
        int count = args.nout; // This number changes as arguments are pushed
        for (int i = 0; i < count; ++i)
            args << (i + 1);
    }
    
    void resetCounter(Args args)
    {
        common_assert(args.nin ==0 && args.nout == 0);
        counter = 0;
        cout << "Counter reset to 0\n";
    }

    void incrementCounter(Args args)
    {
        common_assert_msg(args.nin == 0, "No input arguments allowed");
        common_assert_msg(args.nout == 0 || args.nout == 1, "Need 0 or 1 arguments, given " << args.nout);
        
        counter++;
        cout << "Counter: " << counter << "\n";
        
        if (args.nout == 1)
            args << counter;
    }
};

TestInterface testInterface;

MATLAB_INTERFACE_MAIN(testInterface)
