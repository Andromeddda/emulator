#ifndef HEADER_GUARD_CPU_HPP_INCLUDED
#define HEADER_GUARD_CPU_HPP_INCLUDED

#include <vector>
#include <map>
#include <string>
#include <fstream>

#include "stack.hpp"

class Command;
class Parser;

#define MAX_LINE 100

const int REGS = 6;

class CPU {
private:
	// State while reading byte code
	const char* pos_;
	const char* next_;
	char line_[MAX_LINE];
	unsigned int begin;
	unsigned int end;
public:
	// file with byte-code
	std::ifstream file_;

	stack_ns::Stack<int> stack;
	stack_ns::Stack<int> call_stack;
	
	std::vector<Command*> commands;

	int* registers;
	int pc_register;
	
	CPU(const std::string& filename);

	~CPU();

	void run();
};

#endif //HEADER_GUARD_CPU_HPP_INCLUDED
