#include <iostream>
#include "Lexer.h"
#include "parser.h"
using namespace std;


int main()
{
    lexer lex("test.txt");
    token a;
    //while (a.type != token_type::EndOfFile) {
    //    a = lex.next_token();
    //    cout << a;
    // }

    Parser par(lex);
    auto node = par.parse();
    //testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
    return 0;
}
