#include "Lexer.h"
namespace Lexer {
    Token Lexer::next_token() {
        Token token;

        char sign = this->reader.next_sign();

        while (isspace(sign)) {
            sign = this->reader.next_sign();
        }

        token.line = this->reader.get_current_line_number();
        token.position = this->reader.get_current_sign_position() - 1;

        if (sign == EOF) {
            token.type = token_type::EndOfFile;
        } else if (isalpha(sign) || sign == '_') //delegate to another method
        {
            std::string buffer;

            do {
                buffer.push_back(sign);
                sign = this->reader.next_sign();
            } while (isalnum(sign) || sign == '_');

            this->reader.go_back();
            std::string tempBuff = buffer;

            const auto iterator = keywords.find(tempBuff);
            if (iterator != keywords.end()) {
                token.type = iterator->second;
            } else {
                token.type = token_type::Identifier;
                token.value = buffer;
            }
        } else if (sign == '\"') //what if EOF in string? special characters
        {
            std::string buffer;
            sign = this->reader.next_sign();
            while (sign != '\"'){
                buffer.push_back(sign);
                sign = this->reader.next_sign();
            }
            token.type = token_type::StringLiteral;
            token.value = buffer;
        } else if (isdigit(sign)) //json.org what if number start from 0?
        {
            std::string buffer;
            do {
                buffer.push_back(sign);
                sign = this->reader.next_sign();
            } while (isdigit(sign));

            this->reader.go_back();

            token.type = token_type::IntegerLiteral;
            token.value = buffer;
        } else {
            switch (sign)//change not to use go_back
            {
                case '=': {
                    if (this->reader.next_sign() == '=') {
                        token.type = token_type::Equal;
                    } else {
                        this->reader.go_back();
                        token.type = token_type::Assignment;
                    }
                    break;
                }
                case '<': {
                    if (this->reader.next_sign() == '=') {
                        token.type = token_type::LessEqual;
                    } else {
                        this->reader.go_back();
                        token.type = token_type::Less;
                    }
                    break;
                }
                case '>': {
                    if (this->reader.next_sign() == '=') {
                        token.type = token_type::GreaterEqual;
                    } else {
                        this->reader.go_back();
                        token.type = token_type::Greater;
                    }
                    break;
                }
                case '!': {
                    if (this->reader.next_sign() == '=') {
                        token.type = token_type::NotEqual;
                    } else {
                        this->reader.go_back();
                        token.type = token_type::Negation;
                    }
                    break;
                }
                case ';': {
                    token.type = token_type::Semicolon;
                }
                default: {
                    //auto iterator =
                    if (signs.count(sign) == 1) {
                        token.type = signs.at(sign);
                    } else {
                        token.type = token_type::Invalid;
                    }
                    break;
                }
            }
        }
        return token;
    }

    Lexer::Lexer(const std::string &file) : reader(file) {};
}
