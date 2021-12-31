#include <iostream>
#include "lexer.hpp"

void Token::log() const
{
    const std::string names[] { 
        "[ AOperator ]",
        "[ MOperator ]",
        "[ Semicolon ]",
        "[ OpBracket ]",
        "[ ClBracket ]",
        "[  KeyWord  ]",
        "[  Assign   ]",
        "[  Number   ]",
        "[   None    ]",
        "[    Id     ]",
    };

    std::cout << names[(int)type] << " " << value << std::endl;
};