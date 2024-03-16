#include "utils.hpp"
#include "stack.hpp"
#include "tests.hpp"
#include "test_system.hpp"

#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace stack_ns;
using namespace TestSystem;

//#define TEST

int main(void) {
	std::ofstream out;
	out.open(DEFAULT_OUT_FILE, std::ofstream::out | std::ofstream::trunc);
	if (out.is_open()) {
		out.close();
	}


	Stack<int> s = Stack<int>();
	s.push(1);
	s.push(2);
	std::cout << s.top() << '\n';
	s.pop();
	std::cout << s.top() << '\n';
	s.pop();
	
	#ifdef TEST	
	run_test("empty constructor", test_empty_constructor);
	run_test("copy constructor", test_copy_constructor);
	run_test("move constructor", test_move_constructor);
	run_test("copy assignment", test_copy_assignment);
	run_test("move assignment", test_move_assignment);
	run_test("copy push", test_copy_push);
	run_test("move push", test_move_push);
	run_test("emplace", test_emplace);
	run_test("pop", test_pop);
	run_test("top", test_top);
	#endif // TEST

	return 0;
}