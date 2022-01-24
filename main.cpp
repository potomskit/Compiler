#include <iostream>
#include "lexer/Lexer.h"
#include "interpreter/interpreter.hpp"
using namespace std;

int main(int argc, char** argv)
{
    std::vector<std::string> arguments(argv + 1, argv + argc);
    std::string test = "test.txt";
    std::vector<std::string> parameters;
    parameters.push_back(test);
    Interpreter interpreter(parameters);

    return 0;
}