#include <string>
#include <vector>
#pragma once

class Token
{
public:
    enum class Type
    {
        None,       // '\0'

        Assignment, // '='
        AOperator,  // '+', '-' 
        MOperator,  // '*', '/'
        Semicolon,  // ';'
        OBracket,   // '('
        CBracket,   // ')'
        Keyword,    // 'define'
        Number,     // \d+(\.\d+)?
        Id          // [a-zA-Z]+
    } type;

    Token(size_t line, size_t column, Type type, std::string value) 
        : type(type), value(value), line(line), column(column) {};

    Token(size_t line, size_t column, Type type, char value)
        : Token(line, column, type, std::string(1, value)) {};

    Token(size_t line, size_t column)
        : Token(line, column, Token::Type::None, "") {};

    size_t line, column;
    std::string value;

    void log() const;
};

class Lexer
{
public:
    Lexer(std::string code) : code(code) {};

    Token *next();
    std::string code;

    size_t index = 0, line = 1, column = 1;

private:
    Token *single();
    Token *number();
    Token *word();

    void skipSpaces();

    void comments();
    void blockComment();
    void lineComment();

    bool isNumber() const;
    bool isText() const;

    char following() const;
    char current() const;
    char match();

    [[noreturn]] void fail() const;
};