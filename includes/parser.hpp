#ifndef HEADER_GUARD_PARSER_HPP_INCLUDED
#define HEADER_GUARD_PARSER_HPP_INCLUDED

#include "command.hpp"
#include "cpu.hpp"

#include <regex>
#include <fstream>
#include <iterator>
#include <vector>

#define MAX_LINE_SIZE 100

class Parser {
private:
	std::ifstream file;
	const char* pos;
	const char* end;
	char line[MAX_LINE_SIZE];

	void read_line_from_file();
	bool parse_sequence(std::regex regexp);
	bool parse_sequence(std::regex regexp, std::string& ret);

	bool parse_space_sequence();
	bool parse_newline_sequence();

	CommandArgType parse_arg_type();
	RegisterName parse_register();
	std::string& parse_label();
	int parse_number();

	Command parse_command();
public:
	Parser(const char* filename);
	~Parser() = default;

	Parser() = delete;
	Parser(const Parser& other) = delete;
	Parser(Parser&& other) = delete;
	Parser& operator= (const Parser& other) = delete;
	Parser& operator= (Parser&& other) = delete;

	void parse(CPU& cpu);
};

#endif
