#pragma once
#include <string>
#include "../Utils/ErrorHanlder.h"
#include "../Parser/Parser.cpp"

class Interpreter
{
public:
	Interpreter(std::vector<std::string>& arguments)
	{
		try
	 	{
			if (arguments.size() < 1)
			{
				ErrorHandler::error("No input file. You need to specify file name. Example: ./interpreter test.txt");
			}
			Lexer::Lexer lexer(arguments.at(0));
			Parser parser(lexer);
            Nodes::TreeVisitor visitor;

			auto node = parser.parse();
            if(parser.isError)
            {
                ErrorHandler::error("Termination...", true);
            }
            node->visit(&visitor);
		}
		catch (ErrorHandler::Exception& e)
		{

			ErrorHandler::error("Termination...", true);
		}
	}
};