#ifndef INTERPRETER_LEXER_H
#define INTERPRETER_LEXER_H

#include "Token.h"
#include "../Utils/FileReader.h"
namespace Lexer {
    class Lexer {
        file_reader reader;
    public:
        Token next_token();

        Lexer(const std::string &filename);
    };
}

#endif //INTERPRETER_LEXER_H
