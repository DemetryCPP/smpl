#include <vector>
#include <string>
#include <iostream>

#include "lexer.hpp"
#include "token.hpp"
#include "smpl.hpp"

Lexer::Lexer(std::string value) : value(value) {};

bool Lexer::isNumber()
{ return this->current() >= '0' 
      && this->current() <= '9' 
      || this->current() == '.'; }

bool Lexer::isText()
{ return this->current() >= 'a' 
      && this->current() <= 'z' 
      || this->current() >= 'A' 
      && this->current() <= 'Z'; }

char Lexer::current()
{ return this->value[this->index]; }

char Lexer::peek()
{ return this->value[this->index++]; }

void Lexer::skipWhitespaces()
{ 
    while (this->current() <= ' ')
        this->index++; 
}

Token *Lexer::number()
{
    std::string buffer;
    bool isDouble = false;

    while (this->isNumber())
    {
        buffer += this->peek();
        if (this->current() == '.')
        {
            if (isDouble)
                throw new SMPL::UnexpectedToken(this->index, '.');
            else 
                isDouble = true;
        }
    }

    return new Token(Token::Type::Number, buffer);
}

Token *Lexer::word()
{
    std::string buffer;

    while (this->isText())
        buffer += this->peek();

    if (buffer == "define")
        return new Token(Token::Type::Keyword, buffer);

    return new Token(Token::Type::Id, buffer);
}

Token *Lexer::singleChar()
{
    Token::Type type;

    switch (this->current())
    {
    case '+':
    case '-':
        type = Token::Type::AOperator;
        break;

    case '*':
    case '/':
        type = Token::Type::MOperator;
        break;

    case '(':
        type = Token::Type::OBracket;
        break;

    case ')':
        type = Token::Type::CBracket;
        break;

    case '=':
        type = Token::Type::Equal;
        break;

    case ';':
        type = Token::Type::Semicolon;
        break;

    default:
        throw new SMPL::UnexpectedToken(this->index, this->current());
    }

    return new Token(type, this->peek());
}

Token *Lexer::next()
{
    if (this->index >= this->value.length())
        return new Token();

    this->skipWhitespaces();

    if (this->isNumber()) return this->number();
    if (this->isText())   return this->word();

    return this->singleChar();
}