#include "utils.hpp"
#include "cpu.hpp"
#include "command.hpp"
#include "stack.hpp"

///////////////
// REGISTERS //
///////////////

Registers::Registers() {
	values = new int[REGS];
}


Registers::~Registers() {
	delete[] values;
	values = nullptr;
}

// access to register
int& Registers::operator[] (const RegisterName reg) { return values[reg]; }

// read-
int Registers::operator[] (const RegisterName reg) const { return values[reg]; }


////////////
// LABELS //
////////////

// read-only access by key
// return the pointer
size_t Labels::operator[] (const std::string& name) {
	VERIFY_CONTRACT(labels.contains(name), "ERROR: there is no such label as \"" << name << "\"");
	return labels.at(name);
}

void Labels::add(const std::string& name, size_t value) {
	VERIFY_CONTRACT(!labels.contains(name), "ERROR: label \"" << name << "\" already exist");
	labels.emplace(name, value);
}

bool Labels::contains(const std::string& name) {
	return labels.contains(name);
}

/////////
// CPU //
/////////

CPU::CPU(std::vector<Command*> code, Labels labs) : 
	commands(code), labels(labs), registers(Registers()) {
}

void CPU::run() {
	VERIFY_CONTRACT(labels.contains(BEGIN_L) && labels.contains(END_L), 
		"ERROR: no body of the program. Missing \"BEGIN\" or \"END\" label");
	registers[PC] = labels[BEGIN_L];
	size_t end = labels[END_L];
	while (registers[PC] != static_cast<int>(end)) {
		++registers[PC];
		commands[registers[PC]]->execute(*this);
	}
}

