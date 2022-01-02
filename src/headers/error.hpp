#include <string>
#include "lexer.hpp"
#pragma once

class Error
{
public:
    enum class Type
    {
        UnexpectedToken,
        IsNotAFunction,
        IsNotDefined,
    } type;

    Error(Type type, Token::Pos *pos, std::string token) 
        : pos(pos), token(token), type(type) {};

    Error(Type type, Token::Pos *pos, char token)
        : Error(type, pos, std::string(1, token)) {};

    Token::Pos *pos;
    std::string token;

    std::string format(const std::string &code, const std::string &filename);
};