#include "cpu.hpp"
#include "command.hpp"

#include <iostream>
#include <cstdio>
#include <functional>
#include <map>

// Constructors
Command::Command() : argument(0) { }
Command::Command(int arg) : argument(arg) { }

///////////////////////////
// COMMAND TYPES: NO ARG //
///////////////////////////

class BEGINCommand : public Command {
public:
	BEGINCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg = 0) { return new BEGINCommand(arg); }
	virtual void execute(CPU& cpu) override { 
		cpu.pc_register += 1;
	}
};

class ENDCommand : public Command {
public:
	ENDCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg = 0) { return new ENDCommand(arg); }
	virtual void execute(CPU& cpu) override {
		cpu.pc_register = 0;
	}
};

class POPCommand : public Command {
public:
	POPCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg = 0) { return new POPCommand(arg); }
	virtual void execute(CPU& cpu) override {
		cpu.stack.pop();
		cpu.pc_register += 1;

	}
};

class ADDCommand : public Command {
public:
	ADDCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg = 0) { return new ADDCommand(arg); }
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
	SUBCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg = 0) { return new SUBCommand(arg); }
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
	MULCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg = 0)  { return new MULCommand(arg); }
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
	DIVCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg = 0)  { return new DIVCommand(arg); }
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
	OUTCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg = 0) { return new OUTCommand(arg); }
	virtual void execute(CPU& cpu) override  {
		std::cout << cpu.stack.top() << std::endl;
		cpu.stack.pop();
		cpu.pc_register += 1;
	}
};

class INCommand : public Command {
public:
	INCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg = 0) { return new INCommand(arg); }
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
	RETCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg = 0) { return new RETCommand(arg); }
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
	PUSHCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg)  { return new PUSHCommand(arg); }
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
	PUSHRCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg) { return new PUSHRCommand(arg); }
	virtual void execute(CPU& cpu) override  {
		VERIFY_CONTRACT( (argument >= 0) && (argument <= REGS), "ERROR: invalid register id after command");
		int value = cpu.registers[argument];
		cpu.stack.push(value);
		cpu.pc_register += 1;
	}
};

class POPRCommand : public Command {
public:
	POPRCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg) { return new POPRCommand(arg); }
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
	JMPCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg) { return new JMPCommand(arg); }
	virtual void execute(CPU& cpu) override {
		cpu.pc_register = argument;
	}
};

class JEQCommand : public Command {
public:
	JEQCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg) { return new JEQCommand(arg); }
	virtual void execute(CPU& cpu) override  {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs == lhs) {
			printf("JEQ: %d == %d\n", rhs, lhs);
			cpu.pc_register = argument;
		}
		else {
			cpu.pc_register += 1;
		}
	}
};

class JNECommand : public Command {
public:
	JNECommand(int arg) : Command(arg) {}
	static Command* get_command(int arg) { return new JNECommand(arg); }
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
	JACommand(int arg) : Command(arg) {}
	static Command* get_command(int arg) { return new JACommand(arg); }
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
	JAECommand(int arg) : Command(arg) {}
	static Command* get_command(int arg) { return new JAECommand(arg); } 
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
	JBCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg) { return new JBCommand(arg); }
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
	JBECommand(int arg) : Command(arg) {}
	static Command* get_command(int arg) { return new JBECommand(arg); }
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
	CALLCommand(int arg) : Command(arg) {}
	static Command* get_command(int arg) { return new CALLCommand(arg); }
	virtual void execute(CPU& cpu) override {
		cpu.call_stack.push(cpu.pc_register);
		cpu.pc_register = argument;
	}
};

// Next mapping is used when the parser needs to know what type of argument it needs to parse next
// depending on Command.get_command_arg_type(std::string&)
const std::map<int, std::function<Command*(int)>> command_id_to_function { 
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

Command* Command::get_command(int id, int arg) {
	VERIFY_CONTRACT(command_id_to_function.contains(id), "ERROR: invalid command id");

	int command_arg_family = id / 10;

	// Check if non-argument commands always recieve zero
	if (command_arg_family == 1) {
		VERIFY_CONTRACT(arg == 0, "ERROR: non-zero argument after non-argument command");
	}

	// Check if label is correct
	if (command_arg_family == 2) {
		VERIFY_CONTRACT((arg >= 0), "ERROR: pointer to incorrect label");
	}

	// Check if register id is correct
	if (command_arg_family == 4) {
		VERIFY_CONTRACT( (arg >= 0) && (arg <= REGS), "ERROR: invalid register id after command");
	}

	return command_id_to_function.at(id)(arg);
}
