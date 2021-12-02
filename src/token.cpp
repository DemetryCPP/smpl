#include <vector>
#include <string>
#include <iostream>

#include "lexer.hpp"
#include "token.hpp"
#include "smpl.hpp"

Token::Token(Token::Type type, std::string value) : type(type), value(value) {};
Token::Token(Token::Type type, char c) : Token(type, std::string(1, c)) {};
Token::Token() : Token(Token::Type::Null, "") {};

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