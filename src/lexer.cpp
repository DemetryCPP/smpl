#include <iostream>
#include <map>
#include "lexer.hpp"
#include "smpl.hpp"

using namespace SMPL;
using namespace std;

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

    return new Token(line, column, Token::Type::Number, buffer);
}

Token *Lexer::word()
{
    string buffer;
    size_t column = this->column;
    buffer += match();

    while (isText() || isdigit(current()))
        buffer += match();

    if (buffer == "define")
        return new Token(line, column, Token::Type::Keyword, buffer);

    return new Token(line, column, Token::Type::Id, buffer);
}

Token *Lexer::single()
{
    auto type = map<char, Token::Type>({
        { '+', Token::Type::AOperator },
        { '-', Token::Type::AOperator },
        { '*', Token::Type::MOperator },
        { '/', Token::Type::MOperator },
        { ';', Token::Type::Semicolon },
        { '(', Token::Type::OBracket },
        { ')', Token::Type::CBracket },
        { '=', Token::Type::Equal },
    })[current()];

    if (type == Token::Type::None) fail();

    return new Token(line, column, type, match());
}

void Lexer::skipSpaces()
{ while (current() <= ' ' && index < code.length()) match(); }

bool Lexer::isNumber()
{ return isdigit(current()) || current() == '.'; }

bool Lexer::isText()
{ 
    return current() >= 'a' && current() <= 'z' 
        || current() >= 'A' && current() <= 'Z'
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
{ throw new Error(Error::Type::UnexpectedToken, line, column, current()); }