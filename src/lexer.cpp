#include <iostream>
#include <map>
#include "lexer.hpp"
#include "smpl.hpp"

using namespace SMPL;
using namespace std;
using enum Token::Type;
using enum Error::Type;

Token *Lexer::next()
{
    skipSpaces();

    if (index >= code.length())
        return new Token(line, column);

    if (isNumber()) return number();
    if (isText())   return word();

    return single();
}

Token *Lexer::number()
{
    string buffer;
    bool isDouble = false;
    size_t column = this->column;
    buffer += match();

    while (isNumber())
    {
        if (current() == '.')
            if (isDouble) fail();
            else isDouble = true;

        buffer += match();
    }

    return new Token(line, column, Number, buffer);
}

Token *Lexer::word()
{
    string buffer;
    size_t column = this->column;
    buffer += match();

    while (isText() || isdigit(current()))
        buffer += match();

    if (buffer == "define")
        return new Token(line, column, Keyword, buffer);

    return new Token(line, column, Id, buffer);
}

Token *Lexer::single()
{
    auto type = map<char, Token::Type>({
        { '+', AOperator },
        { '-', AOperator },
        { '*', MOperator },
        { '/', MOperator },
        { ';', Semicolon },
        { '(', OBracket },
        { ')', CBracket },
        { '=', Equal },
    })[current()];

    if (type == None) fail();

    return new Token(line, column, type, match());
}

void Lexer::skipSpaces()
{ while (current() <= ' ' && index < code.length()) match(); }

bool Lexer::isNumber()
{ return isdigit(current()) || current() == '.'; }

bool Lexer::isText()
{ 
    return 'a' <= current() && current() <= 'z' 
        || 'A' <= current() && current() <= 'Z'
        || current() == '_' || current() == '$';
}

char Lexer::current()
{ return code[index]; }

char Lexer::match()
{
    column++;

    if (current() == '\n')
    {
        line++;
        column = 1;
    }

    return code[index++];
}

void Lexer::fail()
{ throw new Error(UnexpectedToken, line, column, current()); }