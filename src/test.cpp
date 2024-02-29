#include "utils.hpp"

#include <cstdlib>
#include <iostream>

int main(void) {

	std::cout << SET_COLOR_RED << "SOME CODE IS RUNNING" << RESET_COLOR << '\n';
	std::cout << SET_COLOR_GREEN << "SOME CODE IS RUNNING" << RESET_COLOR << '\n';
	std::cout << SET_COLOR_CYAN << "SOME CODE IS RUNNING" << RESET_COLOR << '\n';
	std::cout << SET_COLOR_PURPLE << "SOME CODE IS RUNNING" << RESET_COLOR << '\n';
	std::cout << SET_COLOR_YELLOW << "SOME CODE IS RUNNING" << RESET_COLOR << '\n';
	return 0;
}