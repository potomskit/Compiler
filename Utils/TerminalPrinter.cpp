#include "TerminalPrinter.h"
using TerminalPrinter = TerminalPrinter;

const unsigned int TerminalPrinter::getLabelLength(const std::string& message)
{
    return message.length() + 3;
}