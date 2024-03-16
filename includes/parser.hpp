#ifndef HEADER_GUARD_PARSER_HPP_INCLUDED
#define HEADER_GUARD_PARSER_HPP_INCLUDED

#include <regex>
#include <fstream>
#include <iterator>
#include <vector>

#define MAX_LINE_SIZE 100

// wrapper class for label mapping
class Labels {
private:
	// Key-Value map of labels
	std::map<std::string, int> labels;
public:
	Labels() = default;
	
	// read-only access by key
	// return the pointer
	int operator[] (const std::string& name);
	void add(const std::string& name, int value);
	bool contains(const std::string& name);

};

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
	int parse_label();
	int parse_int_number();
	
	Labels labels;
public:
	Parser(const std::string& filename);
	~Parser() = default;

	Parser() = delete;
	Parser(const Parser& other) = delete;
	Parser(Parser&& other) = delete;
	Parser& operator= (const Parser& other) = delete;
	Parser& operator= (Parser&& other) = delete;

	void parse(const std::string& outfile);
};

#endif
