#include <iostream>
#include "lexer.hpp"

void Token::log()
{
    const std::string names[] { 
        "[ AOPER ]",
        "[ MOPER ]",
        "[ SEMIC ]",
        "[ OBRCK ]",
        "[ CBRCK ]",
        "[ KWORD ]",
        "[ EQUAL ]",
        "[  NUM  ]",
        "[  NUL  ]",
        "[  VAR  ]",
    };

    std::cout << names[(int)type] << " " << this->value << std::endl;
};