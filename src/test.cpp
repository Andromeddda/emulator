#include "utils.hpp"
#include "stack.hpp"

#include <cstdlib>
#include <iostream>

using namespace stack_ns;

class Logger {
public:
	Logger() {
		std::cout << "Constructor" << std::endl;
	}

	~Logger() {
		std::cout << "Destructor" << std::endl;
	}
};

int main(void) {

	Stack<Logger> x = Stack<Logger>();
	Stack<Logger> y = x;
	return 0;
}