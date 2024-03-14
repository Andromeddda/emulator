#ifndef HEADER_GUARD_TESTS_HPP_INCLUDED
#define HEADER_GUARD__HPP_INCLUDED

#define DEFAULT_OUT_FILE "hello.txt"

class Logger {
public:
	std::string filename; 
	Logger();
	~Logger();
	Logger(const Logger& other);
	Logger(Logger&& other);
	Logger& operator= (const Logger& other);
	Logger& operator= (Logger&& other);
};


bool test_empty_constructor();
bool test_copy_constructor();
bool test_move_constructor();
bool test_copy_assignment();
bool test_move_assignment();
bool test_copy_push();
bool test_move_push();
bool test_emplace();
bool test_pop();
bool test_top();

#endif //HEADER_GUARD_TESTS_HPP_INCLUDED
