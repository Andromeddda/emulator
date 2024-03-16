#include "parser.hpp"
#include "utils.hpp"

#include <iostream>
#include <string>
#include <regex>

int main(int argc, char** argv) {
	VERIFY_CONTRACT(argc == 2, "Unexpected arguments passed to make code");

	std::string filename(argv[1]);
	std::regex extension = std::regex("[A-Za-z_\\/\\-]+\\.lng");
	VERIFY_CONTRACT(std::regex_match(filename, extension), "Expected .lng file");

	std::cout << SET_COLOR_YELLOW << "Building byte code from " << SET_COLOR_CYAN << filename<< SET_COLOR_YELLOW << "...\n" << RESET_COLOR;

	Parser parser = Parser(filename);
	std::string ofilename = filename.replace(filename.size() - 3, 3, "bcode");
	parser.parse(ofilename);
	std::cout << SET_COLOR_YELLOW << "Building done: " << SET_COLOR_CYAN << ofilename << RESET_COLOR << '\n';
	return 0;
}