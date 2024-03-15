#include "parser.hpp"

Parser::Parser(const char* filename) :
    file_ (std::ifstream(filename, std::ios::in)), pos_ (), end_  ()
{
    VERIFY_CONTRACT(file_.good(), "Unable to open file " << filename);

    // Initialize the first line:
    read_line_from_file();
}

void Parser::read_line_from_file() {
    file_.getline(line_, MAX_LINE_SIZE);

    VERIFY_CONTRACT(
        file_.good() || file_.eof(),
        "Unable to read input line\n");

    pos_ = line_;
    end_ = line_ + std::strlen(line_);
}

bool Parser::parse_pattern(std::regex regexp) {
    std::cmatch match_result{};
    bool match_status = std::regex_search(
        pos_, end_,                              // The iterators limiting the matched sequence
        match_result,                            // Match result
        regexp,                                  // Regular expression specifying the pattern
        std::regex_constants::match_continuous); // Start matching from the beginning

    // Move the iterator on success:
    if (match_status) {
        pos_ = match_result[0].second;
    }

    return match_status;
}

bool Parser::parse_pattern(std::regex regexp, std::string& ret) {
    std::cmatch match_result{};
    bool match_status = std::regex_search(
        pos_, end_,                              // The iterators limiting the matched sequence
        match_result,                            // Match result
        regexp,                                  // Regular expression specifying the pattern
        std::regex_constants::match_continuous); // Start matching from the beginning

    // Move the iterator on success:
    if (match_status) {
        pos_ = match_result[0].second;
        ret = std::string(match_result[0].first, match_result[0].second);
    }

    return match_status;
}

bool Parser::parse_space_sequence() {
    static const std::regex pattern{"[ \t]+"};
    return parse_pattern(pattern);
}

bool Parser::parse_newline_sequence() {
    bool success = (pos_ == end_);
    while (pos_ == end_ && !file_.eof())
    {
        read_line_from_file();
    }
    return success;
}


bool Parser::parse_end_of_file() {
    return file_.eof();
}

std::string& Parser::parse_command_name() {
    static const std::regex pattern{"[A-Z]+"};
    // Skip leading whitespaces (may be none):
    parse_space_sequence();

    // Perform parsing:
    std::string cmd_name;
    bool success = parse_pattern(pattern, cmd_name);
    if (!success)
    {
        throw std::runtime_error("Unable to parse command name!\n");
    }
    return cmd_name;
}

RegisterName Parser::parse_register_name()
{
    static const std::regex pattern{"[A-Z]+"};

    // Skip leading whitespaces:
    bool success = parse_space_sequence();
    if (!success)
    {
        throw std::runtime_error("Expected a space sequence before command name!\n");
    }

    // Perform parsing:
    std::string reg_name;
    success = parse_pattern(pattern, reg_name);
    if (!success)
    {
        throw std::runtime_error("Expected a register name!\n");
    }

    // Throws an exception:
    return get_register_id(reg_name);
}

int Parser::parse_number() {
    static const std::regex pattern{"(\\+|-)?(0|[1-9][0-9]*)"};

    // Skip leading whitespaces:
    bool success = parse_space_sequence();
    if (!success)
    {
        throw std::runtime_error("Expected a space sequence before integral value!\n");
    }

    // Perform parsing:
    std::string val_str;
    success = parse_pattern(pattern, val_str);
    if (!success)
    {
        throw std::runtime_error("Expected an integral value!\n");
    }

    return std::atoi(val_str.c_str());
}
