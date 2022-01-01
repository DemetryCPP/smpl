#include <string>
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

    Error(Type type, size_t line, size_t column, std::string token) 
        : line(line), column(column), token(token), type(type) {};

    Error(Type type, size_t line, size_t column, char token)
        : Error(type, line, column, std::string(1, token)) {};

    size_t line, column;
    std::string token;

    std::string format();
};