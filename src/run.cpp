#include "parser.hpp"
#include "cpu.hpp"
#include <iostream>
#include <string>

int main(void) {
	std::cout << "ENTER FILENAME:\n";

	std::string filename;
	std::cin >> filename;

	CPU cpu = CPU(filename);
	cpu.run();
	return 0;
}