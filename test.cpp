#include <iostream>
#include "lexer.hpp"

int main(int argc, char const *argv[])
{
    std::string a;
    getline(std::cin, a);
    auto lexer = new Lexer(a);
    auto newtoken = lexer->next();
    while (newtoken->name != Token::Name::None)
    {
        newtoken->log();
        newtoken = lexer->next();
    }
    return 0;
}