#include <vector>
#include <string>
#include <iostream>

#include "lexer.hpp"
#include "token.hpp"
#include "smpl.hpp"

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

    std::cout << names[(int)this->type] << " " << this->value << std::endl;
};