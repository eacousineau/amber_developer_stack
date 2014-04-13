#include <vector>
#include <algorithm>

#include "matlab_interface/Command.hpp"

using namespace std;

namespace matlab_interface
{

using std::ostream;

CommandBase::CommandBase(const string &name, const string &description)
    : name(name), description(description)
{ }

string CommandBase::getName() const
{
    return name;
}

int CommandBase::indent = 0;
void CommandBase::doIndent(ostream &os)
{
    for (int i = 0; i < indent; ++i)
        os << "\t";
}
void CommandBase::toString(ostream &os)
{
    doIndent(os);
    os << name << " - " << description << "\n";
}


CommandList::CommandList(const string &name,  const string &description, DispatchFunc onExit)
    : CommandBase(name, description), onExit(onExit)
{
    add(new Command("help", "Print this help", "", boost::bind(&CommandList::printHelp, this)));
}
CommandList::~CommandList()
{
    //for_each(commands.begin(), commands.end(), bl::bind(deletor<CommandBase>, bl::_1));
    for (uint i = 0; i < commands.size(); ++i)
        delete commands[i];
}

void CommandList::add(CommandBase *command)
{
    commands.push_back(command);
}

void CommandList::dispatch(Args args)
{
    // Consumer action, pop front
    if (args.nin == 0)
    {
        printHelp();
        throw logic_error("Please provide a command");
    }
    string str;
    args >> str;

    // Yeehaw
    CommandBase *command = NULL;
    for (uint i = 0; i < commands.size(); ++i)
    {
        if (commands[i]->getName() == str)
        {
            command = commands[i];
            break;
        }
    }
    if (command)
        command->dispatch(args);
    else
    {
        printHelp();
        throw logic_error("Invalid command: " + str);
    }
}

void CommandList::printHelp()
{
    toString(cout);
}

void CommandList::toString(ostream &os)
{
    CommandBase::toString(os);
    indent++;
    //for_each(commands.begin(), commands.end(), bl::bind(&CommandBase::toString, bl::_1)(var(os)));
    for (uint i = 0; i < commands.size(); ++i)
        commands[i]->toString(os);
    // ??? bl::bind(&CommandBase::toString, bl::_1)(os)
    indent--;
}


Command::Command(const string &name, const string &description, const string &expr, DispatchFunc func)
    : CommandBase(name, description), func(func), expr(expr)
{ }

void Command::dispatch(Args args)
{
    func(args);
}

void Command::toString(ostream &os)
{
    CommandBase::toString(os);
    if (expr.size() > 0)
    {
        doIndent(os);
        os << "\t" << expr << "\n";
    }
}

}
