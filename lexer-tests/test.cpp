#include "pch.h"
#include "../Compiler/Lexer.h"
//#include "../Compiler/Lexer.cpp"

const std::string test_file = "example_test.txt";

void write_example_code_to_file(std::string code)
{
	std::ofstream file;
	file.open(test_file);
}
TEST(TestCase, LexerTest) {
	lexer lex("kurde");
	/*token a;
	while (a.type != token_type::EndOfFile) {
		a = lex.next_token();
		std::cout << a;
	}*/
	
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}