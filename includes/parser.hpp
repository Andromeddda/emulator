#ifndef HEADER_GUARD_PARSER_HPP_INCLUDED
#define HEADER_GUARD_PARSER_HPP_INCLUDED

#include <regex>
#include <fstream>
#include <iterator>
#include <vector>
#include <map>
#include <set>

#define MAX_LINE_SIZE 100

class Parser {
private:
	std::ifstream file_;

	const char* pos_;
	const char* end_;
	char line_[MAX_LINE_SIZE];

	int command_line_number;

	void read_line_from_file();
	bool parse_pattern(std::regex regexp);
	bool parse_pattern(std::regex regexp, std::string& ret);

	bool parse_space_sequence();
	bool parse_newline_sequence();
	bool parse_end_of_file();

	bool parse_label_declaration();
	int parse_command();
	int parse_register();
	int parse_int_number();
	std::string parse_label();

	std::map<std::string, int> declared_labels;
	std::map<long int, std::string> used_labels;
public:
	Parser(const std::string& filename);
	~Parser();

	Parser() = delete;
	Parser(const Parser& other) = delete;
	Parser(Parser&& other) = delete;
	Parser& operator= (const Parser& other) = delete;
	Parser& operator= (Parser&& other) = delete;

	void parse(const std::string& outfile);
};

#endif
