#include "command.hpp"
#include "cpu.hpp"
#include <iostream>
#include <cstdio>
#include <functional>
#include <map>

IntArgCommand::IntArgCommand(const int& val) : argument(val) {}

RegisterArgCommand::RegisterArgCommand(const RegisterName& reg_name) : argument(reg_name) {}

LabelArgCommand::LabelArgCommand(const std::string& label_name) : argument(label_name) {}

///////////////////////////
// COMMAND TYPES: NO ARG //
///////////////////////////

class POPCommand : public NoArgCommand {
public:
	POPCommand() : NoArgCommand() { };
	static Command* get_command() { return new POPCommand(); }
	virtual void execute(CPU& cpu) override {
		cpu.stack.pop();
	}
};

class ADDCommand : public NoArgCommand {
public:
	ADDCommand() = default;
	static Command* get_command() { return new ADDCommand(); }
	virtual void execute(CPU& cpu) override {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		cpu.stack.push(rhs + lhs);
	}
};

class SUBCommand : public NoArgCommand {
public:
	SUBCommand() = default;
	static Command* get_command() { return new SUBCommand(); }
	virtual void execute(CPU& cpu) override  {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		cpu.stack.push(rhs - lhs);
	}
};

class MULCommand : public NoArgCommand {
public:
	MULCommand() = default;
	static Command* get_command()  { return new MULCommand(); }
	virtual void execute(CPU& cpu) override {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		cpu.stack.push(rhs * lhs);
	}
};

class DIVCommand : public NoArgCommand {
public:
	DIVCommand() = default;
	static Command* get_command()  { return new DIVCommand(); }
	virtual void execute(CPU& cpu) override {
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		cpu.stack.push(rhs / lhs);
	}
};

class OUTCommand : public NoArgCommand {
public:
	OUTCommand() = default;
	static Command* get_command() { return new OUTCommand(); }
	virtual void execute(CPU& cpu) override  {
		printf("%d\n", cpu.stack.top());
		cpu.stack.pop();
	}
};

class INCommand : public NoArgCommand {
public:
	INCommand() = default;
	static Command* get_command() { return new INCommand(); }
	virtual void execute(CPU& cpu) override {
		int value;
		int correct = scanf("%d", &value);
		VERIFY_CONTRACT(correct == 1, "ERROR: invalid input in IN command");
		cpu.stack.push(value);
	}
};

class RETCommand : public NoArgCommand {
public:
	RETCommand() = default;
	static Command* get_command() { return new RETCommand(); }
	virtual void execute(CPU& cpu) override {
		cpu.registers[PC] = (cpu.call_stack.top());
		cpu.call_stack.pop();
	}
};


////////////////////////////
// COMMAND TYPES: INT ARG //
////////////////////////////

class PUSHCommand : public IntArgCommand {
public:
	PUSHCommand(const int& arg) : IntArgCommand(arg) {}
	static Command* get_command(const int& arg)  { return new PUSHCommand(arg); }
	virtual void execute(CPU& cpu) override {
		cpu.stack.push(argument);
	}
};

/////////////////////////////////
// COMMAND TYPES: REGISTER ARG //
/////////////////////////////////

class PUSHRCommand : public RegisterArgCommand {
public:
	PUSHRCommand(const RegisterName& reg_name) : RegisterArgCommand(reg_name) {}
	static Command* get_command(const RegisterName& reg_name) { return new PUSHRCommand(reg_name); }
	virtual void execute(CPU& cpu) override  {
		cpu.stack.push(cpu.registers[argument]);
	}
};

class POPRCommand : public RegisterArgCommand {
public:
	POPRCommand(const RegisterName& reg_name) : RegisterArgCommand(reg_name) {}
	static Command* get_command(const RegisterName& reg_name) { return new POPRCommand(reg_name); }
	virtual void execute(CPU& cpu) override {
		cpu.registers[argument] = cpu.stack.top();
		cpu.stack.pop();
	}
};

//////////////////////////////
// COMMAND TYPES: LABEL ARG //
//////////////////////////////

class JMPCommand : public LabelArgCommand {
public:
	JMPCommand(const std::string& label_name) : LabelArgCommand(label_name) {}
	static Command* get_command(const std::string label_name) { return new JMPCommand(label_name); }
	virtual void execute(CPU& cpu) override {
		VERIFY_CONTRACT(cpu.labels.contains(argument), "ERROR: JMP to non-existing label");
		cpu.registers[PC] = (cpu.labels[argument]);
	}
};

class JEQCommand : public LabelArgCommand {
public:
	JEQCommand(const std::string& label_name) : LabelArgCommand(label_name) {}
	static Command* get_command(const std::string label_name) { return new JEQCommand(label_name); }
	virtual void execute(CPU& cpu) override  {
		VERIFY_CONTRACT(cpu.labels.contains(argument), "ERROR: JEQ to non-existing label");
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs == lhs) {
			cpu.registers[PC] = (cpu.labels[argument]);
		}
	}
};

class JNECommand : public LabelArgCommand {
public:
	JNECommand(const std::string& label_name) : LabelArgCommand(label_name) {}
	static Command* get_command(const std::string label_name) { return new JNECommand(label_name); }
	virtual void execute(CPU& cpu) override  {
		VERIFY_CONTRACT(cpu.labels.contains(argument), "ERROR: JEQ to non-existing label");
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs != lhs) {
			cpu.registers[PC] = (cpu.labels[argument]);
		}
	}
};

class JACommand : public LabelArgCommand {
public:
	JACommand(const std::string& label_name) : LabelArgCommand(label_name) {}
	static Command* get_command(const std::string label_name) { return new JACommand(label_name); }
	virtual void execute(CPU& cpu) override {
		VERIFY_CONTRACT(cpu.labels.contains(argument), "ERROR: JA to non-existing label");
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs > lhs) {
			cpu.registers[PC] = (cpu.labels[argument]);
		}
	}
};

class JAECommand : public LabelArgCommand {
public:
	JAECommand(const std::string& label_name) : LabelArgCommand(label_name) {}
	static Command* get_command(const std::string label_name) { return new JAECommand(label_name); } 
	virtual void execute(CPU& cpu) override {
		VERIFY_CONTRACT(cpu.labels.contains(argument), "ERROR: JEQ to non-existing label");
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs >= lhs) {
			cpu.registers[PC] = (cpu.labels[argument]);
		}
	}
};

class JBCommand : public LabelArgCommand {
public:
	JBCommand(const std::string& label_name) : LabelArgCommand(label_name) {}
	static Command* get_command(const std::string label_name) { return new JBCommand(label_name); }
	virtual void execute(CPU& cpu) override {
		VERIFY_CONTRACT(cpu.labels.contains(argument), "ERROR: JB to non-existing label");
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs < lhs) {
			cpu.registers[PC] = (cpu.labels[argument]);
		}
	}
};

class JBECommand : public LabelArgCommand {
public:
	JBECommand(const std::string& label_name) : LabelArgCommand(label_name) {}
	static Command* get_command(const std::string label_name) { return new JBECommand(label_name); }
	virtual void execute(CPU& cpu) override {
		VERIFY_CONTRACT(cpu.labels.contains(argument), "ERROR: JBE to non-existing label");
		auto rhs = cpu.stack.top();
		cpu.stack.pop();
		auto lhs = cpu.stack.top();
		cpu.stack.pop();
		if (rhs <= lhs) {
			cpu.registers[PC] = (cpu.labels[argument]);
		}
	}
};

class CALLCommand : public LabelArgCommand {
public:
	CALLCommand(const std::string& label_name) : LabelArgCommand(label_name) {}
	static Command* get_command(const std::string label_name) { return new CALLCommand(label_name); }
	virtual void execute(CPU& cpu) override {
		VERIFY_CONTRACT(cpu.labels.contains(argument), "ERROR: CALL of non-existing label");
		cpu.call_stack.push(cpu.labels[argument]);
		cpu.registers[PC] = (cpu.call_stack.top());
	}
};

// Next mapping is used when the parser needs to know what type of argument it needs to parse next
// depending on Command.get_command_arg_type(std::string&)

const std::map<std::string, std::function<Command*()>> no_arg_cmds { 
	{"POP", POPCommand::get_command},
 	{"ADD", ADDCommand::get_command},
 	{"SUB", SUBCommand::get_command},
 	{"MUL", MULCommand::get_command},
 	{"DIV", DIVCommand::get_command},
 	{"OUT", OUTCommand::get_command}, 
 	{"IN", INCommand::get_command},
 	{"RET", RETCommand::get_command}
};

const std::map<std::string, std::function<Command*(const int&)>> int_arg_cmds {
	{"PUSH", PUSHCommand::get_command } 
};

const std::map<std::string, std::function<Command*(const std::string&)>> label_arg_cmds { 
	{"JMP", JMPCommand::get_command },
	{"JEQ", JEQCommand::get_command },
	{"JNE", JNECommand::get_command },
	{"JA", JACommand::get_command },
	{"JAE", JAECommand::get_command },
	{"JB", JBCommand::get_command },
	{"JBE", JBECommand::get_command },
	{"CALL", CALLCommand::get_command }
};

const std::map<std::string, std::function<Command*(const RegisterName&)>> reg_arg_cmds { 
	{"POPR", POPRCommand::get_command}, 
	{"PUSHR", PUSHRCommand::get_command}, 
};

CommandArgType get_command_arg_type(const std::string& name) {
	if (int_arg_cmds.contains(name)) return IntArg;
	if (no_arg_cmds.contains(name)) return NoArg;
	if (label_arg_cmds.contains(name)) return LabelArg;
	if (reg_arg_cmds.contains(name)) return RegArg;
	TERMINATE("ERROR: no such command");
}

