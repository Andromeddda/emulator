#pragma once

#include <string>
#include <vector>

class CPU;

class Command {
public:
	Command() = delete;
	virtual void execute(CPU& cpu);
	static Command* get_command(const std::string);
};

class NoArgCommand: public Command {
public:
	NoArgCommand();
};

class LabelArgCommand: public Command {
protected:
	const std::string label;
public:
	LabelArgCommand(const std::string& name);
};