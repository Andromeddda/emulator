#ifndef HEADER_GUARD_COMMAND_HPP_INCLUDED
#define HEADER_GUARD_COMMAND_HPP_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "cpu.hpp"

///////////////////
// COMMAND CLASS //
///////////////////

class Command {
protected:
	// Whatever the argument is (label, register or value), 
	// it will be presented as integer in byte-code after parsing
	const int argument;
public:
	Command();
	Command(int arg);
	~Command() = default;
	virtual void execute(CPU& cpu) = 0;
	static Command* get_command(int id, int argument);
};


#endif //HEADER_GUARD_COMMAND_HPP_INCLUDED
