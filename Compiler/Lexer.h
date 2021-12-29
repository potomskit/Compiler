#pragma once
#include "Token.h",
#include "File_reader.h"


class lexer {
public: 
    token next_token();
    lexer(const std::string& filename);
    file_reader reader;
};
