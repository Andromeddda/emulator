#include "cpu.hpp"
#include "command.hpp"

#include <iostream>
#include <cstdio>
#include <functional>
#include <map>

// Constructors
Command::Command() : argument(0) { }
Command::Command(const int& arg) : argument(arg) { }

///////////////////////////
// COMMAND TYPES: NO ARG //
///////////////////////////

class BEGINCommand : public Command {
public:
	BEGINCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg = 0) { return new BEGINCommand(arg); }
	virtual void execute(CPU& cpu) override { 
		cpu.pc_register += 1;
	}
};

class ENDCommand : public Command {
public:
	ENDCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg = 0) { return new ENDCommand(arg); }
	virtual void execute(CPU& cpu) override {
		cpu.pc_register = 0;
	}
};

class POPCommand : public Command {
public:
	POPCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg = 0) { return new POPCommand(arg); }
	virtual void execute(CPU& cpu) override {
		cpu.stack.pop();
		cpu.pc_register += 1;

	}
};

class ADDCommand : public Command {
public:
	ADDCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg = 0) { return new ADDCommand(arg); }
	virtual void execute(CPU& cpu) override {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		cpu.stack.push(rhs + lhs);
		cpu.pc_register += 1;
	}
};

class SUBCommand : public Command {
public:
	SUBCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg = 0) { return new SUBCommand(arg); }
	virtual void execute(CPU& cpu) override  {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		cpu.stack.push(rhs - lhs);
		cpu.pc_register += 1;
	}
};

class MULCommand : public Command {
public:
	MULCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg = 0)  { return new MULCommand(arg); }
	virtual void execute(CPU& cpu) override {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		cpu.stack.push(rhs * lhs);
		cpu.pc_register += 1;
	}
};

class DIVCommand : public Command {
public:
	DIVCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg = 0)  { return new DIVCommand(arg); }
	virtual void execute(CPU& cpu) override {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		cpu.stack.push(rhs / lhs);
		cpu.pc_register += 1;
	}
};

class OUTCommand : public Command {
public:
	OUTCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg = 0) { return new OUTCommand(arg); }
	virtual void execute(CPU& cpu) override  {
		printf("%d\n", cpu.stack.top());
		cpu.stack.pop();
		cpu.pc_register += 1;
	}
};

class INCommand : public Command {
public:
	INCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg = 0) { return new INCommand(arg); }
	virtual void execute(CPU& cpu) override {
		int value;
		int correct = scanf("%d", &value);
		VERIFY_CONTRACT(correct == 1, "ERROR: invalid input in IN command");
		cpu.stack.push(value);
		cpu.pc_register += 1;
	}
};

class RETCommand : public Command {
public:
	RETCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg = 0) { return new RETCommand(arg); }
	virtual void execute(CPU& cpu) override {
		cpu.pc_register = cpu.call_stack.top();
		cpu.call_stack.pop();
		cpu.pc_register += 1;
	}
};

////////////////////////////
// COMMAND TYPES: INT ARG //
////////////////////////////

class PUSHCommand : public Command {
public:
	PUSHCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg)  { return new PUSHCommand(arg); }
	virtual void execute(CPU& cpu) override {
		cpu.stack.push(argument);
		cpu.pc_register += 1;
	}
};

/////////////////////////////////
// COMMAND TYPES: REGISTER ARG //
/////////////////////////////////

class PUSHRCommand : public Command {
public:
	PUSHRCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg) { return new PUSHRCommand(arg); }
	virtual void execute(CPU& cpu) override  {
		VERIFY_CONTRACT( (argument >= 0) && (argument <= REGS), "ERROR: invalid register id after command");
		cpu.stack.push(cpu.registers[argument]);
		cpu.pc_register += 1;
	}
};

class POPRCommand : public Command {
public:
	POPRCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg) { return new POPRCommand(arg); }
	virtual void execute(CPU& cpu) override {
		VERIFY_CONTRACT( (argument >= 0) && (argument <= REGS), "ERROR: invalid register id after command");
		cpu.registers[argument] = cpu.stack.top();
		cpu.stack.pop();
		cpu.pc_register += 1;
	}
};

//////////////////////////////
// COMMAND TYPES: LABEL ARG //
//////////////////////////////

class JMPCommand : public Command {
public:
	JMPCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg) { return new JMPCommand(arg); }
	virtual void execute(CPU& cpu) override {
		cpu.pc_register = argument;
	}
};

class JEQCommand : public Command {
public:
	JEQCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg) { return new JEQCommand(arg); }
	virtual void execute(CPU& cpu) override  {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs == lhs) {
			cpu.pc_register = argument;
		}
		else {
			cpu.pc_register += 1;
		}
	}
};

class JNECommand : public Command {
public:
	JNECommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg) { return new JNECommand(arg); }
	virtual void execute(CPU& cpu) override  {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs != lhs) {
			cpu.pc_register = argument;
		}
		else {
			cpu.pc_register += 1;
		}
	}
};

class JACommand : public Command {
public:
	JACommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg) { return new JACommand(arg); }
	virtual void execute(CPU& cpu) override {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs > lhs) {
			cpu.pc_register = argument;
		}
		else {
			cpu.pc_register += 1;
		}
	}
};

class JAECommand : public Command {
public:
	JAECommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg) { return new JAECommand(arg); } 
	virtual void execute(CPU& cpu) override {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs >= lhs) {
			cpu.pc_register = argument;
		}
		else {
			cpu.pc_register += 1;
		}
	}
};

class JBCommand : public Command {
public:
	JBCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg) { return new JBCommand(arg); }
	virtual void execute(CPU& cpu) override {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs < lhs) {
			cpu.pc_register = argument;
		}
		else {
			cpu.pc_register += 1;
		}
	}
};

class JBECommand : public Command {
public:
	JBECommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg) { return new JBECommand(arg); }
	virtual void execute(CPU& cpu) override {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs <= lhs) {
			cpu.pc_register = argument;
		}
		else {
			cpu.pc_register += 1;
		}
	}
};

class CALLCommand : public Command {
public:
	CALLCommand(const int& arg) : Command(arg) {}
	static Command* get_command(const int& arg) { return new CALLCommand(arg); }
	virtual void execute(CPU& cpu) override {
		cpu.call_stack.push(cpu.pc_register);
		cpu.pc_register = argument;
	}
};

// Next mapping is used when the parser needs to know what type of argument it needs to parse next
// depending on Command.get_command_arg_type(std::string&)
const std::map<int, std::function<Command*(const int&)>> command_id_to_function { 
	{10, BEGINCommand::get_command},
	{11, POPCommand::get_command},
 	{12, ADDCommand::get_command},
 	{13, SUBCommand::get_command},
 	{14, MULCommand::get_command},
 	{15, DIVCommand::get_command},
 	{16, OUTCommand::get_command}, 
 	{17, INCommand::get_command},
 	{18, RETCommand::get_command},
 	{19, ENDCommand::get_command},

 	{20, CALLCommand::get_command },
	{21, JMPCommand::get_command },
	{22, JEQCommand::get_command },
	{23, JNECommand::get_command },
	{24, JACommand::get_command },
	{25, JAECommand::get_command },
	{26, JBCommand::get_command },
	{27, JBECommand::get_command },
	
	{30, PUSHCommand::get_command },

	{40, POPRCommand::get_command}, 
	{41, PUSHRCommand::get_command},
};

Command* Command::get_command(const int& id, const int& arg, const unsigned& line) {
	VERIFY_CONTRACT(command_id_to_function.contains(id), "ERROR: invalid command id");

	int command_arg_family = id / 10;

	// Check if non-argument commands always recieve zero
	if (command_arg_family == 1) {
		VERIFY_CONTRACT(arg == 0, "ERROR: non-zero argument after non-argument command");
	}

	// Check if label is correct
	if (command_arg_family == 2) {
		VERIFY_CONTRACT((arg >= 0) && (arg < static_cast<int>(line)), "ERROR: pointer to undefined or incorrect label");
	}

	// Check if register id is correct
	if (command_arg_family == 4) {
		VERIFY_CONTRACT( (arg >= 0) && (arg <= REGS), "ERROR: invalid register id after command");
	}

	return command_id_to_function.at(id)(arg);
}
