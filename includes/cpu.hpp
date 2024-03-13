#pragma once

#include <vector>
#include <map>

#include "stack.hpp"

class Command;

enum RegisterName { PC, AX, BX, CX, DX, EX, FX };
const unsigned REGS = 7;
const std::string BEGIN_L = "BEGIN";
const std::string END_L = "END";

// Registers is a wrapper-class of int[7] array.
// It is used so that we can access registers only by their names, but not indexes
class Registers {
private:
	int* values;
public:
	Registers();
	~Registers();

	// access to register
	int& operator[] (const RegisterName reg);

	// read-only access
	int operator[] (const RegisterName reg) const;

};

class Labels {
private:
	// Key-Value map of labels
	std::map<std::string, size_t> labels;
public:
	Labels() = default;
	
	// read-only access by key
	// return the pointer
	size_t operator[] (const std::string& name);
	void add(const std::string& name, size_t value);
	bool contains(const std::string& name);

};

class CPU {
public:
	// These are used while parsing and stored unchanged while running the code 
	std::vector<Command*> commands; // parsed code
	Labels labels; // parsed map of labels as keys and indexes of commands as values 

	// These are empty till parsing is done and are used in runtime
	Registers registers;
	stack_ns::Stack<int> stack;
	stack_ns::Stack<size_t> call_stack;

	CPU(std::vector<Command*> code, Labels labs);
	~CPU() = default;

	void run();
};

