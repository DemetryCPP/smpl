#include <string>
#include <vector>
#pragma once

class Token
{
public:
    struct Pos
    { size_t line, column, lineIndex; };

    enum class Type
    {
        Undefined,
        None,       // '\0'

        Assignment, // '='
        AOperator,  // '+', '-' 
        MOperator,  // '*', '/'
        Semicolon,  // ';'
        OBracket,   // '('
        CBracket,   // ')'
        Define,    // 'define'
        Number,     // \d+(\.\d+)?
        Id          // [a-zA-Z]+
    } type;

    Token(Type type, Pos *pos, std::string value) 
        : type(type), value(value), pos(pos) {};

    Token(Type type, Pos *pos, char value)
        : Token(type, pos, std::string(1, value)) {};

    Token(Pos *pos)
        : Token(Token::Type::None, pos, "") {};

    std::string value;
    Pos *pos;

    void log() const;

    friend bool operator==(Token *, Type);
    friend bool operator==(Token *, std::string);
};

class Lexer
{
public:
    Lexer(std::string code) : code(code) {};

    Token *next();
    std::string code;

    size_t index = 0, 
            line = 1, 
          column = 1, 
   lastLineBegin = 0;

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

    bool isBlockComment() const;
    bool isLineComment() const;

    char following() const;
    char current() const;
    char match();

    Token::Pos *pos() const;
    [[noreturn]] void fail() const;
};