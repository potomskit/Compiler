#include "ErrorHanlder.h"

#include <sstream>
#include <iostream>

using ErrorHandler = ErrorHandler;
using Exception = ErrorHandler::Exception;

//more types of exceptions
Exception::Exception(const std::string& error) noexcept :
        error(error)
{}

const char* Exception::what() const noexcept
{
    return this->error.c_str();
}

void ErrorHandler::debugFatal(const std::string& message)
{
    ErrorHandler::print(ErrorHandler::getLabelLength("Fatal"), message);

    throw ErrorHandler::Exception(message);
}

void ErrorHandler::error(const std::string& message, const bool& noThrow)
{
    ErrorHandler::print(ErrorHandler::getLabelLength("Error"), message);

    if (noThrow)
    {
        return;
    }
     
    throw ErrorHandler::Exception(message);
}

void ErrorHandler::warning(const std::string& message)
{
    ErrorHandler::print(ErrorHandler::getLabelLength("Warn"), message);
}

void ErrorHandler::notice(const std::string& message)
{
    ErrorHandler::print(ErrorHandler::getLabelLength("Note"), message);
}

void ErrorHandler::print(const unsigned int& labelShift, const std::string& message)
{
    std::stringstream messageStream;
    messageStream << message;

    std::string line;

    std::getline(messageStream, line);
    std::cout << line << std::endl;

    while (std::getline(messageStream, line))
    {
        std::cout << std::string(labelShift, ' ') << line << std::endl;
    }
}