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

    Error(Type type, Token::Pos *pos, std::string token, std::string hint)
        : pos(pos), token(token), type(type), hint(hint) {};
        
    Error(Type type, Token::Pos *pos, std::string token)
        : Error(type, pos, token, "") {};

    Error(Type type, Token::Pos *pos, char token)
        : Error(type, pos, std::string(1, token), "") {};

    Token::Pos *pos;
    std::string token, hint;

    std::string format(const std::string &code, const std::string &filename);
};