#include "parser.hpp"
#include "command.hpp"
#include "cpu.hpp"
#include "utils.hpp"

const std::map<std::string, int> str_to_reg {
    {"AX", 0},
    {"BX", 1},
    {"CX", 2},
    {"DX", 3},
    {"EX", 4},
    {"FX", 5}
};

int get_register_id(const std::string& name) {
    VERIFY_CONTRACT(str_to_reg.contains(name), "ERROR: cannot get register id. No such register");
    return str_to_reg.at(name);
}

// All id-s are two-digit numbers
// Commands with no argument        start with "1"
// Commands with label argument     start with "2"
// Commands with integer argument   start with "3"
// Commands with register argumen   start with "4"
const std::map<std::string, int> command_name_to_id {
    {"BEGIN", 10},
    {"POP", 11},
    {"ADD", 12},
    {"SUB", 13},
    {"MUL", 14},
    {"DIV", 15},
    {"OUT", 16},
    {"IN",  17},
    {"RET", 18},
    {"END", 19},

    {"CALL", 20},
    {"JMP", 21},
    {"JEQ", 22},
    {"JNE", 23},
    {"JA",  24},
    {"JAE", 25},
    {"JB",  26},
    {"JBE", 27},

    {"PUSH", 30},

    {"POPR", 40},
    {"PUSHR", 41}
};

int get_command_id(std::string& name) {
    VERIFY_CONTRACT(command_name_to_id.contains(name), "ERROR: cannot get command id. No such command");
    return command_name_to_id.at(name);
}

////////////
////////////
// PARSER //
////////////

// Constructor
Parser::Parser(const std::string& filename) :
    file_ (std::ifstream(filename, std::ios::in)), pos_ (), end_(), command_line_number(0) {
    VERIFY_CONTRACT(file_.good(), "Unable to open file " << filename);

    // Initialize the first line:
    read_line_from_file();
}

Parser::~Parser() {
    //declared_labels.erase();
    //used_labels.erase();
    declared_labels.clear();
    used_labels.clear();
}

// Put line in private buffer
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

bool Parser::parse_label_declaration() {
    static const std::regex pattern{"[A-Za-z_\\-]+:"};

    // Skip leading whitespaces
    parse_space_sequence();

    // Perform parsing:
    std::string label_str;
    bool success = parse_pattern(pattern, label_str);
    if (!success) {
        return false;
    }

    label_str.pop_back();
    declared_labels[label_str] =  command_line_number;

    return true;
}

int Parser::parse_command() {
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
    return get_command_id(cmd_name);
}

int Parser::parse_register() {
    static const std::regex pattern{"[A-Z]+"};

    // Skip leading whitespaces:
    bool success = parse_space_sequence();
    if (!success)
    {
        throw std::runtime_error("Expected a space sequence before register!\n");
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

int Parser::parse_int_number() {
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

std::string Parser::parse_label() {
    static const std::regex pattern{"[A-Za-z_\\-]+"};

    // Skip leading whitespaces:
    bool success = parse_space_sequence();
    if (!success)
    {
        throw std::runtime_error("Expected a space sequence before label name!\n");
    }

    // Perform parsing:
    std::string label_str;
    success = parse_pattern(pattern, label_str);
    if (!success) {
        throw std::runtime_error("Expected label name!\n");
    }

    return label_str;
}

void Parser::parse(const std::string& outfile) {
    std::ofstream out;
    out.open(outfile);

    if (out.is_open()) {
        while (!parse_end_of_file()) {
            // skip all empty lines at every step
            parse_newline_sequence();

            if (parse_label_declaration()) continue;
            else {
                int cmd_id = parse_command();

                // switch case may fall through T_T 
                if (cmd_id / 10 == 1) {
                    out << cmd_id << " " << 0 << std::endl;
                }
                else if (cmd_id / 10 == 2) {
                    // write command and 50 whitespaces as 
                    out << cmd_id << ' ';

                    // store the pair position-label
                    used_labels[out.tellp()] = parse_label();
                    
                    // write 50 whitespaces as buffer
                    out << std::string(50, ' ') << std::endl;
                }
                else if (cmd_id / 10 == 3) {
                    out << cmd_id << " " << parse_int_number() << std::endl;
                }
                else if (cmd_id / 10 == 4) {
                    //std::cout << "POPR OR PUSHR!!!\n";
                    out << cmd_id << " " << parse_register() << std::endl;
                }
                else {
                    throw std::runtime_error("Unexpected error");
                }

                ++command_line_number;
            }
        } // while

        // Run throug pairs position-label 
        for (const auto& [key, value] : used_labels) {
            // Check if label is declared
            VERIFY_CONTRACT(declared_labels.contains(value), "ERROR: reference to undefined label " << value);

            // Go to the position of byte code where it suppose to be used
            out.seekp(key);

            // Write the pointer
            out << declared_labels.at(value);
        }
    } // if
} // parse
