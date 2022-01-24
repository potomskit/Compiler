#ifndef INTERPRETER_FILEREADER_H
#define INTERPRETER_FILEREADER_H


#include <fstream>

class file_reader
{
public:
    file_reader(const std::string& file);

    char next_sign();
    void go_back();
    unsigned int& get_current_line_number();
    unsigned int& get_current_sign_position();

private:
    std::ifstream handler;
    unsigned int current_line_number = 0;
    unsigned int current_sign_position = 0;
    char previous_sign = 0;
};

#endif //INTERPRETER_FILEREADER_H
