#ifndef INTERPRETER_TRACER_H
#define INTERPRETER_TRACER_H

#include <string>
#include "TerminalPrinter.h"


class Tracer : public TerminalPrinter
{
public:
    Tracer(const bool& enabled = false);

    void reset();

    void enter(const std::string& message = "");
    void leave(const std::string& message = "leave");
    void info(const std::string& message);

private:
    const bool enabled;
    unsigned int nestingLevel = 0;

    void printNesting();
};


#endif //INTERPRETER_TRACER_H
