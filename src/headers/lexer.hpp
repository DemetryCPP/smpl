#include <vector>
#include <string>

#include "token.hpp"

#pragma once

class Lexer
{
private:
    bool isNumber();
    bool isText();

    char current();
    char previous();
    char peek();

    void skipWhitespaces();

    Token *number();
    Token *word();
    Token *singleChar();

    void unexpectedToken();

public:
    Lexer(std::string);

    size_t index = 0;

    Token *next();
    std::string value;
};