#include <iostream>
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
    buffer += match();
    size_t column = this->column;

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
    buffer += match();
    size_t column = this->column;

    while (isText())
        buffer += match();

    if (buffer == "define")
        return new Token(line, column, Token::Type::Keyword, buffer);

    return new Token(line, column, Token::Type::Id, buffer);
}

Token *Lexer::single()
{
    Token::Type type;

    switch (current())
    {
    case '+':
    case '-': type = Token::Type::AOperator; break;

    case '*':
    case '/': type = Token::Type::MOperator; break;

    case '(': type = Token::Type::OBracket;  break;
    case ')': type = Token::Type::CBracket;  break;
    case '=': type = Token::Type::Equal;     break;
    case ';': type = Token::Type::Semicolon; break;

    default: fail();
    }

    return new Token(line, column, type, match());
}

void Lexer::skipSpaces()
{ while (current() <= ' ' && index < code.length()) match(); }

bool Lexer::isNumber()
{ return isdigit(current()) || current() == '.'; }

bool Lexer::isText()
{ 
    return current() >= 'a' && current() <= 'z' 
        || current() >= 'A' && current() <= 'Z';
}

char Lexer::current()
{ return code[index]; }

char Lexer::match()
{
    column++;

    if (current() == '\n')
    {
        line++;
        column = 0;
    }

    return code[index++];
}

void Lexer::fail()
{ throw new Error(Error::Type::UnexpectedToken, line, column, current()); }