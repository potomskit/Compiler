#include <iostream>
#include "Lexer.h"

using namespace std;


int main()
{
    lexer lex("test.txt");
    token a;
    while (a.type != token_type::EndOfFile) {
        a = lex.next_token();
        cout << a;
    }
    //testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
    return 0;
}
