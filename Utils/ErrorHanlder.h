#ifndef INTERPRETER_ERRORHANLDER_H
#define INTERPRETER_ERRORHANLDER_H

#include <exception>
#include <string>
#include "TerminalPrinter.h"

class ErrorHandler : public TerminalPrinter
{
public:
    class Exception : public std::exception
    {
        friend class ErrorHandler;

    public:
        virtual const char* what() const noexcept;

    private:
        Exception(const std::string& error) noexcept;

        const std::string error;
    };

    static void debugFatal(const std::string& message);
    static void error(const std::string& message, const bool& noThrow = false);
    static void warning(const std::string& message);
    static void notice(const std::string& message);

    static void print(const unsigned int& labelShift, const std::string& message);
};



#endif //INTERPRETER_ERRORHANLDER_H
