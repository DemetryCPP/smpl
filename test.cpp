#include <iostream>
#include "parser.hpp"

int main(int argc, char const *argv[])
{
    std::string a;
    getline(std::cin, a);

    auto lexer = new Lexer(a);
    auto parser = new SMPL::AST::Parser(lexer);

    return 0;
}