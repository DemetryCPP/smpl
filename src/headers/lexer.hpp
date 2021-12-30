#include "token.hpp"
#pragma once

class Lexer
{
public:
    Lexer(std::string code) : code(code) {};

    Token *next();
    std::string code;

    size_t index = 0;

private:
    Token *single();
    Token *number();
    Token *word();

    void skipSpaces();

    bool isNumber();
    bool isText();

    char current();
    char match();

    void fail();
};