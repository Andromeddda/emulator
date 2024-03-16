#include "utils.hpp"
#include "cpu.hpp"
#include "command.hpp"
#include "stack.hpp"

#include <regex>

/////////
// CPU //
/////////

CPU::CPU(const std::string& filename) : pos_(), next_(), begin(0), end(0) {
	// Check if the extension is correct
	std::regex extension {"[A-Za-z_\\/\\-]+\\.bcode"};
	bool correct_file_extension = std::regex_match(filename, extension);
	VERIFY_CONTRACT(correct_file_extension, "ERROR: incorrect file extension. Expected .bcode file");

	file_ = std::ifstream(filename);
	registers = new int[REGS];
	pc_register = 0;
}

CPU::~CPU() {
	if (registers != nullptr) {
		delete[] registers;
		registers = nullptr;
	}

	if (file_.is_open()) {
		file_.close();
	}
}

// read the .bcode file and run the byte code
void CPU::run() {
	unsigned current_line = 0;

	while(!file_.eof() && !end) {
		file_.getline(line_, MAX_LINE);

		VERIFY_CONTRACT(
		    file_.good() || file_.eof(),
		    "ERROR: Unable to read line from .bcode file\n");

		pos_ = line_;
		next_ = line_ + std::strlen(line_);

		int command_id, argument;

		int correct = sscanf(line_, "%d %d", &command_id, &argument);

		// printf("read %d %d\n", command_id, argument);
		VERIFY_CONTRACT(correct, "ERROR: invalid .bcode file format. Unexpected symbol or incorrect id");
		commands.push_back(Command::get_command(command_id, argument, current_line));

		if (command_id == 10) begin = current_line;
		if (command_id == 19) end = current_line;

		++current_line;
	}

	pc_register = begin;
	while (pc_register != static_cast<int>(end)) {
		//printf("execute %d\n", pc_register);
		commands[pc_register]->execute(*this);
	}
}

