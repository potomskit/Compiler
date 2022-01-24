#ifndef INTERPRETER_PARSERTOKEN_H
#define INTERPRETER_PARSERTOKEN_H
enum ParserTokenType{
    Equal = 10,
    NotEqual = 11,
    Less = 12,
    Greater = 13,
    LessEqual = 14,
    GreaterEqual = 15,
    Plus = 16,
    Minus = 17,
    Multiply = 18,
    Divide = 19,
    IntegerLiteral = 20,
    Invalid = 30,
    StringLiteral = 34,

};

class ParserToken{
public:
    ParserToken() = default;
    ParserToken(const ParserTokenType& type) : type(type) {}// change contructor and set public variables to private and then do setters
    ParserToken(const Lexer::Token token){
        line = token.line;
        position = token.position;
        type = (ParserTokenType)token.type;
        value = token.value;
    }
    ParserTokenType type = ParserTokenType::Invalid;
    std::string value = "";
    unsigned int line = 0;
    unsigned int position = 0;
};

#endif //INTERPRETER_PARSERTOKEN_H
