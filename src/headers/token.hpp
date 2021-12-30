#include <vector>
#include <string>
#pragma once

class Token
{
public:
    enum class Type
    {
        AOperator,  // '+', '-' 
        MOperator,  // '*', '/'
        Semicolon,  // ';'
        OBracket,   // '('
        CBracket,   // ')'
        Keyword,    // 'define'
        Equal,      // '='
        Number,     // \d+('.'\d+)?
        Null,       // '\0'
        Id          // [a-zA-Z]+
    };

    Token(Type type, std::string value)
        : type(type), value(value) {};
    Token(Type type, char value)
        : Token(type, std::string(1, value)) {};
    Token() : Token(Token::Type::Null, "") {};

    Type type;
    std::string value;

    void log();
};
