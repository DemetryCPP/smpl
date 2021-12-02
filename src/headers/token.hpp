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
        Number,     // numbers
        Null,       // null token
        Id          // identificators
    };

    Token(Token::Type, std::string);
    Token(Token::Type, char);
    Token();

    Token::Type type;
    std::string value;

    void log();
};
