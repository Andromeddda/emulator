#ifndef HEADER_GUARD_UTILS_HPP_INCLUDED
#define HEADER_GUARD_UTILS_HPP_INCLUDED

#include <iostream>
#include <cstdlib>

#define VERIFY_CONTRACT(contract, message) \
if (!(contract)) { \
	std::cout << SET_COLOR_RED << message << RESET_COLOR << '\n'; \
	exit(1); \
}

#define TERMINATE(message) \
	std::cout << SET_COLOR_RED << message << RESET_COLOR << '\n'; \
	exit(1);

#define SET_COLOR_RED 		"\033[1;31m"
#define SET_COLOR_GREEN 	"\033[1;32m"
#define SET_COLOR_YELLOW 	"\033[1;33m"
#define SET_COLOR_PURPLE 	"\033[1;35m"
#define SET_COLOR_CYAN 		"\033[1;36m"
#define RESET_COLOR 		"\033[0m"

#endif //HEADER_GUARD_UTILS_HPP_INCLUDED
