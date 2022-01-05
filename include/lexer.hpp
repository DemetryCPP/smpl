#include "token.hpp"
#pragma once

class Lexer
{
public:
    Lexer(const std::string &code)
        : code(code) {};

    Token::Pos *pos = new Token::Pos{1, 0, 0};
    const std::string &code;

    Token *next();
    Token::Pos *getPos() const;
    size_t index = 0;

private:
    Token *special();
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

    [[noreturn]] void fail() const;
};