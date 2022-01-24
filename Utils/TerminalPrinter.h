#ifndef INTERPRETER_TERMINALPRINTER_H
#define INTERPRETER_TERMINALPRINTER_H


#include <string>
#include <map>


class TerminalPrinter
{
protected:
    static const unsigned int getLabelLength(const std::string& message);
};


#endif //INTERPRETER_TERMINALPRINTER_H
