#ifndef HEADER_GUARD_COMMAND_HPP_INCLUDED
#define HEADER_GUARD_COMMAND_HPP_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "cpu.hpp"

class CPU;

enum CommandArgType { NoArg, LabelArg, IntArg, RegArg };

///////////////////
// COMMAND CLASS //
///////////////////

class Command {
public:
	Command() = default;
	virtual ~Command() = default;
	virtual void execute(CPU& cpu) = 0;
	static CommandArgType get_command_arg_type(const std::string& name);
};

////////////////////////////
// COMMAND FAMILY CLASSES //
////////////////////////////

class NoArgCommand: public Command {
public:
	NoArgCommand() = default;
	virtual ~NoArgCommand() = default;
};

class IntArgCommand: public Command {
protected:
	const int argument;
public:
	IntArgCommand(const int& val);
	virtual ~IntArgCommand() = default;
};

class RegisterArgCommand: public Command {
protected:
	const RegisterName argument;
public:
	RegisterArgCommand(const RegisterName& reg_name);
	virtual ~RegisterArgCommand() = default;
};

class LabelArgCommand: public Command {
protected:
	const std::string argument;
public:
	LabelArgCommand(const std::string& label_name);
	virtual ~LabelArgCommand() = default;
};

#endif //HEADER_GUARD_COMMAND_HPP_INCLUDED
