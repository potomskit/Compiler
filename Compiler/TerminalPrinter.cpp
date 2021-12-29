#include "TerminalPrinter.h"

#include <sstream>
#include <iostream>

using TerminalPrinter = TerminalPrinter;

const unsigned int TerminalPrinter::getLabelLength(const std::string& message)
{
    return message.length() + 3;
}