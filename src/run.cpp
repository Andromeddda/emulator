#include "parser.hpp"
#include "cpu.hpp"
#include <iostream>
#include <string>

int main(int argc, char** argv) {
	VERIFY_CONTRACT(argc == 2, "Unexpected arguments passed to make code");

	std::string filename(argv[1]);
	std::regex extension = std::regex("[A-Za-z_\\/\\-]+\\.bcode");
	VERIFY_CONTRACT(std::regex_match(filename, extension), "Expected .bcode file");

	CPU cpu = CPU(filename);

	std::cout << SET_COLOR_YELLOW << "Running program " << SET_COLOR_CYAN << filename << SET_COLOR_YELLOW << "...\n" << RESET_COLOR;
	cpu.run();
	return 0;
}