#include "smpl.hpp"

using namespace SMPL;
using Pos = Token::Pos;
using enum Token::Type;
using enum Error::Type;

Token *Lexer::next()
{
    skipSpaces();
    comments();

    if (index >= code.length())
        return new Token(pos());

    if (isNumber()) return number();
    if (isText())   return word();

    return single();
}

Token *Lexer::number()
{
    string buffer;
    bool isDouble = false;
    Token::Pos *pos = this->pos();
    buffer += match();

    while (isNumber())
    {
        if (current() == '.')
            if (isDouble) fail();
            else isDouble = true;

        buffer += match();
    }

    return new Token(Number, pos, buffer);
}

Token *Lexer::word()
{
    string buffer;
    Token::Pos *pos = this->pos();
    buffer += match();

    while (isText() || isdigit(current()))
        buffer += match();

    if (buffer == "define")
        return new Token(Define, pos, buffer);

    return new Token(Id, pos, buffer);
}

Token *Lexer::single()
{
    auto type = map<char, Token::Type>({
        { '=', Assignment },
        { '+', AOperator },
        { '-', AOperator },
        { '*', MOperator },
        { '/', MOperator },
        { ';', Semicolon },
        { '(', OBracket },
        { ')', CBracket },
    })[current()];

    return new Token(type, pos(), match());
}

void Lexer::comments()
{
    if (isLineComment()) lineComment();
    else if (isBlockComment()) blockComment();

    skipSpaces();

    if (isLineComment() || isBlockComment())
        comments();
}

void Lexer::lineComment()
{
    while (current() != '\n' && index < code.length())
        match();
}

void Lexer::blockComment()
{
    match(); match();
    while (current() != '*' || following() != '/')
        match();
    match(); match();
}

void Lexer::skipSpaces()
{ 
    while (current() <= ' ' && index < code.length()) 
        match();
}

bool Lexer::isNumber() const
{ return isdigit(current()) || current() == '.'; }

bool Lexer::isText() const
{ 
    return 'a' <= current() && current() <= 'z' 
        || 'A' <= current() && current() <= 'Z'
        || current() == '_' || current() == '$';
}

char Lexer::current() const
{ return code[index]; }

char Lexer::following() const
{ return code[index + 1]; }

char Lexer::match()
{
    column++;

    if (current() == '\n')
    {
        line++;
        column = 1;
        lastLineBegin = index + 1;
    }

    return code[index++];
}

bool Lexer::isBlockComment() const
{ return current() == '/' && following() == '*'; }

bool Lexer::isLineComment() const
{ return current() == '/' && following() == '/'; }

Pos *Lexer::pos() const
{ return new Pos{line, column, lastLineBegin}; }

void Lexer::fail() const
{ throw new Error(UnexpectedToken, pos(), current()); }