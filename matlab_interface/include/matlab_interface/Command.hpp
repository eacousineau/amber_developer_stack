#ifndef MATLAB_COMMAND_H
#define MATLAB_COMMAND_H

#include <vector>
#include <boost/function.hpp>
#include <boost/bind.hpp>

#include <matlab_interface/Args.hpp>

// http://cc.byexamples.com/2008/06/16/gnu-readline-implement-custom-auto-complete/
// http://cc.byexamples.com/2008/06/13/gnu-readline-how-to-keep-a-history-list-of-entered-command-lines/

// TODO Generalize this to use for command line

// For dispatch, make an abstract Arg class, can be istream, or MatlabArgs

#define MATLAB_INTERFACE_BIND(type, method) \
	boost::bind(&type::method, this, _1)

namespace matlab_interface
{

typedef boost::function<void(Args)> DispatchFunc;

class CommandBase
{
protected:
    std::string name, description;
    static int indent;
public:
    CommandBase(const std::string &name, const std::string &description);
    std::string getName() const;
    virtual void dispatch(Args args) = 0;
    static void doIndent(std::ostream &os);
    virtual void toString(std::ostream &os);
};


class Command : public CommandBase
{
    DispatchFunc func;
    std::string expr;
public:
    Command(const std::string &name, const std::string &description, const std::string &expr, DispatchFunc func);
    void dispatch(Args args);
    void toString(std::ostream &os);
};


class CommandList : public CommandBase
{
    std::vector<CommandBase *> commands;
    DispatchFunc onExit;
public:
    CommandList(const std::string &name,  const std::string &description, DispatchFunc onExit = NULL);
    ~CommandList();

    void add(CommandBase *command);
    void dispatch(Args args);
    void printHelp();
    void toString(std::ostream &os);
};

}

#endif // MATLAB_COMMAND_H
