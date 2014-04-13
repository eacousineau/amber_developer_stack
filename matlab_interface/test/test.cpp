#include <boost/bind.hpp>

#include "amber/matlab/Interface.hpp"

using namespace amber::easy;

// I want to pass an entry point in matlab for it to call...
// http://dirkgorissen.com/2012/02/06/bridging-python-and-matlab-passing-callbacks-and-optimization/
// Looking at: https://github.com/dgorissen/pymatopt.git

namespace amber
{
namespace matlab
{

class TestInterface : public Interface
{
    int counter;
public:
    TestInterface()
        : counter(0)
    {
        commands.add(new Command("meh", "Meh, do something", "[obj] = f(A, b)",
                boost::bind(&TestInterface::meh, this, _1)));
        commands.add(new Command("hai", "Say hello", "",
                boost::bind(&TestInterface::hai, this, _1)));
        commands.add(new Command("incr", "Increment (show persistence)", "",
                boost::bind(&TestInterface::incr, this, _1)));
    }

    void load()
    {
    }

    void meh(Args args)
    {
        assert_ex_msg(args.nin == 2 && args.nout == 1, "Need two inputs and one output");

        MatrixXd A, C;
        double b;
        args >> A >> b;

        C = -b * A.transpose();
        cout << "A: " << A << "\nb: " << b << "\nC: " << C << "\n";

        Struct obj;
        Cell blar;
        blar << A << b << C << "Hello";
        obj.set("name", "Test");
        obj.set("C", C);
        obj.set("blar", to_mex(blar));

        // TODO Make struct stuff more elegant with const' stuff later
        args.push(to_mex(obj));
    }

    void hai(Args args)
    {
        cout << "You gave me " << args.nin << " input and " << args.nout << " output arguments.\n";
    }

    void incr(Args args)
    {
        counter++;
        cout << "Counter: " << counter << "\n";
    }
};

TestInterface testInterface;

}
}

AMBER_MATLAB_MAIN(amber::matlab::testInterface);

// How can this be linked against libamber_matlab.a and not have a multiple definition linker error?


