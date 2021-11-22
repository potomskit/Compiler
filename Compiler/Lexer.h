#pragma once
#include "Token.h",
#include "File_reader.h"


class lexer {
public:
    token next_token();
    lexer(const std::string& filename);
    const std::string get_line(const std::streampos& line_position);
    file_reader reader;
};
