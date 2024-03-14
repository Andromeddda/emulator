#include "test_system.hpp"
#include "stack.hpp"
#include "tests.hpp"
#include "utils.hpp"

#include <vector>
#include <fstream>

using namespace stack_ns;
using namespace TestSystem;
using namespace std;

Logger::Logger() {
	filename = DEFAULT_OUT_FILE;
	std::ofstream out;
	out.open(filename, std::ofstream::app);
	if (out.is_open()) {
		out << "Default constructor\n";
	}
	out.close();
}

Logger::Logger(const Logger& other) : filename(other.filename) {
	std::ofstream out;
	out.open(filename, std::ofstream::app);
	if (out.is_open()) {
		out << "Copy constructor\n";
	}
	out.close();
}

Logger::Logger(Logger&& other) : filename(std::move(other.filename)) {
	std::ofstream out;
	out.open(filename, std::ofstream::app);
	if (out.is_open()) {
		out << "Move constructor\n";
	}
	out.close();
}

Logger& Logger::operator= (const Logger& other) {
	filename = other.filename;
	std::ofstream out;
	out.open(filename, std::ofstream::app);
	if (out.is_open()) {
		out << "Copy assignment\n";
	}
	out.close();
	return *this;
}

Logger& Logger::operator= (Logger&& other) {
	filename = std::move(other.filename);
	std::ofstream out;
	out.open(filename, std::ofstream::app);
	if (out.is_open()) {
		out << "Move assignment\n";
	}
	out.close();
	return *this;
}

Logger::~Logger() {
	std::ofstream out;
	out.open(filename, std::ofstream::app);
	if (out.is_open()) {
		out << "Destructor\n";
	}
	out.close();
	filename = "";
}

bool test_empty_constructor() {
	std::ofstream out;
	out.open(DEFAULT_OUT_FILE, std::ofstream::app);
	if (out.is_open()) {
		//out << "\ntest_empty_constructor:\n";
		Stack<Logger> stack = Stack<Logger>();
		//out << '\n';
		out.close();
	}
	return true;
}

bool test_copy_constructor() {
	std::ofstream out;
	out.open(DEFAULT_OUT_FILE, std::ofstream::app);
	if (out.is_open()) {
		//out << "\ntest_copy_constructor:\n";
		const Stack<Logger> stack = Stack<Logger>();
		Stack<Logger> stack_copy = Stack<Logger>(stack);

		//out << '\n';
		out.close();
	}
	return true;
}

bool test_move_constructor() {
	std::ofstream out;
	out.open(DEFAULT_OUT_FILE, std::ofstream::app);
	if (out.is_open()) {
		//out << "\ntest_move_constructor:\n";

		Stack<Logger> stack = Stack<Logger>();
		Stack<Logger> stack_moved = Stack<Logger>(stack);

		//out << '\n';
		out.close();
	}
	return true;
}

bool test_copy_assignment() {
	std::ofstream out;
	out.open(DEFAULT_OUT_FILE, std::ofstream::app);
	if (out.is_open()) {
		//out << "\ntest_copy_assignment:\n";

		const Stack<Logger> stack = Stack<Logger>();
		Stack<Logger> stack_copy = stack;

		//out << '\n';
		out.close();
	}
	

	return true;
}

bool test_move_assignment() {
	std::ofstream out;
	out.open(DEFAULT_OUT_FILE, std::ofstream::app);
	if (out.is_open()) {
		//out << "\ntest_move_assignment:\n";

		Stack<Logger> stack = Stack<Logger>();
		Stack<Logger> stack_copy = stack;

		//out << '\n';
		out.close();
	}
	
	return true;
}

bool test_copy_push() {
	std::ofstream out;
	out.open(DEFAULT_OUT_FILE, std::ofstream::app);
	if (out.is_open()) {
		//out << "\ntest_copy_push:\n";

		const Logger logger;
		Stack<Logger> stack;
		stack.push(logger);

		//out << '\n';
		out.close();
	}
	
	return true;
}

bool test_move_push() {
	std::ofstream out;
	out.open(DEFAULT_OUT_FILE, std::ofstream::app);
	if (out.is_open()) {
		//out << "\ntest_move_push:\n";

		Logger logger;
		Stack<Logger> stack;
		stack.push(logger);

		//out << '\n';
		out.close();
	}
	return true;
}

bool test_emplace() {
	std::ofstream out;
	out.open(DEFAULT_OUT_FILE, std::ofstream::app);
	if (out.is_open()) {
		//out << "\ntest_emplace:\n";

		Stack<Logger> stack;
		stack.emplace();

		//out << '\n';
		out.close();
	}

	return true;
}

bool test_pop() {
	std::ofstream out;
	out.open(DEFAULT_OUT_FILE, std::ofstream::app);
	if (out.is_open()) {
		//out << "\ntest_pop:\n";

		Logger logger;
		Stack<Logger> stack;
		stack.push(logger);
		stack.pop();

		//out << '\n';
		out.close();
	}
	
	return true;
}
bool test_top() {
	std::ofstream out;
	out.open(DEFAULT_OUT_FILE, std::ofstream::app);
	if (out.is_open()) {
		//out << "\ntest_top:\n";

		Logger logger;
		Stack<Logger> stack;
		stack.push(logger);
		stack.pop();

		//out << '\n';
		out.close();
	}
	return true;
}
