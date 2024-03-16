#include "parser.hpp"
#include <iostream>
#include <string>

int main(void) {

	std::cout << "ENTER INPUT FILENAME:\n";

	std::string filename;
	std::cin >> filename;

	Parser parser = Parser(filename);

	std::cout << "\nENTER OUTPUT FILENAME:\n";
	std::string ofilename;
	std::cin >> ofilename;

	parser.parse(ofilename);
	return 0;
}