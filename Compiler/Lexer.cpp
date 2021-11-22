#include "Lexer.h"

token lexer::next_token() {
    token token;


    char sign = this->reader.next_sign();

    while (isspace(sign))
    {
        sign = this->reader.next_sign();
    }

    token.line = this->reader.get_current_line_number();
    token.position = this->reader.get_current_sign_position() - 1;

    if (sign == EOF)
    {
        token.type = token_type::EndOfFile;
    }
    else if (sign == '\n') {
        token.type = token_type::NewLine;
    }
    else if (isalpha(sign) || sign == '_')
    {
	    std::string buffer;

        do
        {
            buffer.push_back(sign);
            sign = this->reader.next_sign();
        } while (isalnum(sign) || sign == '_');

        this->reader.go_back();
	    std::string tempBuff = buffer;

        if (keywords.count(tempBuff) == 1)
        {
            token.type = keywords.at(tempBuff);
        }
        else
        {
            token.type = token_type::Identifier;
            token.value = buffer;
        }
    }
    else if (sign == '\"')
    {
	    std::string buffer;
        do
        {
            buffer.push_back(sign);
            sign = this->reader.next_sign();
        } while (sign != '\"');

        buffer.push_back(sign);
        token.type = token_type::StringLiteral;
        token.value = buffer;
    }
    else if (isdigit(sign))
    {
	    std::string buffer;
        do
        {
            buffer.push_back(sign);
            sign = this->reader.next_sign();
        } while (isdigit(sign));

        this->reader.go_back();

        token.type = token_type::IntegerLiteral;
        token.value = buffer;
    }
    else
    {
        switch (sign)
        {
        case '=':
        {
            if (this->reader.next_sign() == '=')
            {
                token.type = token_type::Equal;
            }
            else
            {
                this->reader.go_back();
                token.type = token_type::Assignment;
            }
            break;
        }
        case '<':
        {
            if (this->reader.next_sign() == '=')
            {
                token.type = token_type::LessEqual;
            }
            else
            {
                this->reader.go_back();
                token.type = token_type::Less;
            }
            break;
        }
        case '>':
        {
            if (this->reader.next_sign() == '=')
            {
                token.type = token_type::GreaterEqual;
            }
            else
            {
                this->reader.go_back();
                token.type = token_type::Greater;
            }
            break;
        }
        case '!':
        {
            if (this->reader.next_sign() == '=')
            {
                token.type = token_type::NotEqual;
            }
            else
            {
                this->reader.go_back();
                token.type = token_type::Negation;
            }
            break;
        }
        default:
        {
            if (signs.count(sign) == 1)
            {
                token.type = signs.at(sign);
            }
            else
            {
                token.type = token_type::Invalid;
            }
            break;
        }
        }
    }
    return token;
}

const std::string lexer::get_line(const std::streampos& line_position)
{
    return this->reader.get_line(line_position);
}

lexer::lexer(const std::string& file) :reader(file) {};

