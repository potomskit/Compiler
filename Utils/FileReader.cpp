#include "FileReader.h"
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
    char sign = this->handler.get();
    if(sign == '\r' || sign == '\n'){
        this->current_line_number++;
        this->current_sign_position = 0;
    }
    else
    {
        this->previous_sign = sign;
        this->current_sign_position++;
    }
    if(sign == '\\')
    {
        char tmp = this->handler.get();
        if(tmp == '\\')
        {
            sign='\\';
        }
        else if(tmp == 'n' || tmp =='r')
        {
            sign='\n';
        }

        this->previous_sign=sign;
        this->current_sign_position++;
    }
    return sign;
}

void file_reader::go_back()
{
    this->handler.unget().unget();

    this->previous_sign = this->handler.get();

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