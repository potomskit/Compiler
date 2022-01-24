cmake_minimum_required(VERSION 3.20)
project(interpreter)

set(CMAKE_CXX_STANDARD 14)

add_executable(interpreter main.cpp lexer/Lexer.cpp lexer/Lexer.h Utils/TerminalPrinter.cpp Utils/TerminalPrinter.h Utils/FileReader.cpp Utils/FileReader.h Utils/ErrorHanlder.cpp Utils/ErrorHanlder.h lexer/Token.h Parser/Parser.cpp Tree/Node.cpp Tree/Node.h Tree/Nodes.h "Tree/TreeVisitor.cpp" "interpreter/interpreter.hpp" "Tree/TreeVisitor.h"  "Tree/Nodes.cpp" interpreter/StandardLib.cpp interpreter/StandardLib.h Parser/ParserToken.h)
