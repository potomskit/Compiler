#pragma once
#define TKOM_CONFIG_TERMINALCOLORS true
#include <string>
#include <map>


class TerminalPrinter
{
protected:
    static const unsigned int getLabelLength(const std::string& message);
};
   

