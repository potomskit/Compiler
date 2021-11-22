#include "File_reader.h"
#include <iostream>
#include <string>

file_reader::file_reader(const std::string& file) : handler(file)
{
    if (this->handler.fail())
    {
        std::cout << "no such file \n";
    }
}

char file_reader::next_sign()
{
    while (true)
    {
        char sign = this->handler.get();
        this->previous_sign = sign;
        this->current_sign_position++;

        return sign;
    }
}

void file_reader::go_back()
{
    this->handler.unget().unget();

    this->previous_sign = this->handler.get();
    const int peek = this->handler.peek();

    if (peek == '\n' || peek == '\r')
    {
        this->previous_sign = this->handler.get();
        return;
    }

    this->current_sign_position--;
}

unsigned int& file_reader::get_current_line_number()
{
    return this->current_line_number;
}

unsigned int& file_reader::get_current_sign_position()
{
    return this->current_sign_position;
}

std::string file_reader::get_line(const std::streampos& line_position)
{
    const std::streampos current_position = this->handler.tellg();
    std::string line;

    this->handler.seekg(line_position);

    getline(this->handler, line);

    this->handler.seekg(current_position);

    return line;
}