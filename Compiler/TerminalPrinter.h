#pragma once
#define TKOM_CONFIG_TERMINALCOLORS true
#include <string>
#include <map>


class TerminalPrinter
{
protected:
    static const std::map<std::string, const unsigned int>& getColors();

    static const std::string colorize(const std::string& message, const std::string& color);
    static void printLabel(const std::string& message, const std::string& color = "");
    static const unsigned int getLabelLength(const std::string& message);
};
   

