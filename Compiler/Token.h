#pragma once
#include <string>
#include <unordered_map>
#include <fstream>

enum token_type : unsigned int
{
    Comma = 0,
    Dot = 1,
    If = 2,
    While = 3,
    Else = 4,
    Return = 5,
    Negation = 6,
    Assignment = 7,
    Or = 8,
    And = 9,
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
    EndOfFile = 21,
    Function = 23,
    LeftParen = 24,
    RightParen = 25,
    Elif = 26,
    LeftBracket = 27,
    RightBracket = 28,
    Class = 29,
    Invalid = 30,
    Identifier = 31,
    Continue = 32,
    Break = 33,
    StringLiteral = 34,
	Undefined = 35,
    Semicolon = 36,
};

class token {
public:
    token() = default;
    token(const token_type& type) : type(type) {} // change contructor and set public variables to private and then do setters
    token_type type = token_type::Invalid;
    std::string value = "";
    unsigned int line = 0;
    unsigned int position = 0;
    unsigned int line_start = 0;
	
};

const std::unordered_map<std::string, token_type> keywords =
{
        {"def", token_type::Function},
        {"id", token_type::If},
        {"while", token_type::While},
        {"elif", token_type::Elif},
        {"else", token_type::Else},
        {"and", token_type::And},
        {"or", token_type::Or},
        {"class", token_type::Class},
        {"continue", token_type::Continue},
        {"break", token_type::Break},
        {"return", token_type::Return},
};

const std::unordered_map<char, token_type> signs =
{
        {'(', token_type::LeftParen},
        {')', token_type::RightParen},
        {'{', token_type::LeftBracket},
        {'}', token_type::RightBracket},
        {',', token_type::Comma},
        {'+', token_type::Plus},
        {'-', token_type::Minus},
        {'*', token_type::Multiply},
        {'/', token_type::Divide},
        {'.', token_type::Dot},
        {'!', token_type::Negation},
		{';', token_type::Semicolon},
};

const std::unordered_map<unsigned int, std::string> tokenTypeNames = {
       { 0,  "Comma"},
       { 1,  "Dot"},
       { 2,  "If"},
       { 3,  "While"},
       { 4,  "Else"},
       { 5,  "Return"},
       { 6,  "Negation"},
       { 7,  "Assignment"},
       { 8,  "Or"},
       { 9,  "And"},
       { 10, "Equal"},
       { 11, "NotEqual"},
       { 12, "Less"},
       { 13, "Greater"},
       { 14, "LessEqual"},
       { 15, "GreaterEqual"},
       { 16, "Plus"},
       { 17, "Minus"},
       { 18, "Multiply"},
       { 19, "Divide"},
       { 20, "IntegerLiteral"},
       { 21, "EndOfFile"},
       { 23, "Function"},
       { 24, "LeftParen"},
       { 25, "RightParen"},
       { 26, "Elif"},
       { 27, "LeftBracket"},
       { 28, "RightBracket"},
       { 29, "Class"},
       { 30, "Invalid"},
       { 31, "Identifier"},
       { 32, "Continue"},
       { 33, "Break"},
       { 34, "StringLiteral"},
       { 35, "Undefined"},
	   { 36, "Semicolon"},
};

inline std::ostream& operator<< (std::ostream& os, token const& token) {
    os << token.type;
    os << " ";
    os << token.value;
    os << "\n";
    return os;
}
inline const std::string getTokenTypeName(const token_type& tokenType)
{
    return tokenTypeNames.at(static_cast<unsigned int>(tokenType));
};