#include <iostream>
#include <map>
#include "lexer.hpp"
#include "error.hpp"
#include "utils.hpp"

using namespace std;
using namespace SMPL;
using enum Error::Type;
using enum Token::Name;

Token *Lexer::next()
{
    skipSpaces();
    comments();

    if (index >= code.length())
        return new Token{None, getPos()};

    if (isNumber()) return number();
    if (isText())   return word();

    return special();
}

Token *Lexer::special()
{
    auto pos = getPos();
    string result;


    // equality
    if (current() == '=' && following() == '=')
        return new Token(Operator, pos, string(1, match()) + match());

    // all comparsion operators
    else if (contains("<>!", current()))
    {
        result = match();
        if (current() == '=')
            result += match();

        return new Token(Operator, pos, result);
    }
    // arithmetic and logic operators
    else if (contains("+-*/\\%^&|", current()))
        return new Token(Operator, pos, match());

    // special chars
    else if (contains("=;(){},", current()))
        return new Token(map<char, Token::Name>({
            { '=', Assignment },
            { ';', Semicolon  },
            { '(', ORBracket  },
            { ')', CRBracket  },
            { '{', OFBracket  },
            { '}', CFBracket  },
            { ',', Comma      }
        })[match()], pos);

    fail();
}

Token *Lexer::number()
{
    string result;
    auto pos = getPos();

    if (current() == '0' && contains("box", following()))
    {
        result += match();

        auto numabc = map<char, string>({
            { 'b', "01" },
            { 'o', "01234567" },
            { 'x', "0123456789ABCDEFabcdef" }
        })[current()];

        result += match();

        if (!contains(numabc, current()))
            fail();

        while (contains(numabc, current()))
            result += match();

        return new Token(Number, pos, result);
    }

    bool isDouble = false;

    while (isNumber() || current() == '\'')
    {
        if (current() == '.')
            if (isDouble) fail();
            else isDouble = true;

        result += match();
    }

    if (current() == 'e')
    {
        result += match();

        if (contains("+-", current()))
            result += match();

        if (!isdigit(current()))
            fail();

        while (isdigit(current()))
            result += match();
    }

    return new Token(Number, pos, result);
}

Token *Lexer::word()
{
    string result;
    auto pos = getPos();

    while (isText())
        result += match();

    auto type = map<string, Token::Name>({
        { "const",  Modifer },
        { "let",    Modifer },
        { "define", Define  },
        { "while",  While   },
        { "else",   Else    },
        { "if",     If      }
    })[result];

    if (type == Undefined)
        return new Token(Id, pos, result);

    return new Token(type, pos);
}

void Lexer::skipSpaces()
{ while (current() <= ' ' && index < code.length()) match(); }

void Lexer::comments()
{
    if (isBlockComment()) blockComment();
    else if (isLineComment()) lineComment();

    skipSpaces();
    if (isBlockComment() || isLineComment())
        comments();
}

void Lexer::blockComment()
{
    match(); match();
    while (current() != '*' && following() != '/')
        match();
    match(); match();
}

void Lexer::lineComment()
{ while (current() != '\n' && index < code.length()) match(); }

bool Lexer::isNumber() const
{ return isdigit(current()) || current() == '.'; }

bool Lexer::isText() const
{
    return 'a' <= current() && current() <= 'z'
        || 'A' <= current() && current() <= 'Z'
        || current() == '_' && current() == '$'; 
}

bool Lexer::isBlockComment() const
{ return current() == '/' && following() == '*'; }

bool Lexer::isLineComment() const
{ return current() == '/' && following() == '/'; }

char Lexer::following() const
{ return code[index + 1]; }

char Lexer::current() const
{ return code[index]; }

char Lexer::match()
{ return code[index++]; }

Token::Pos *Lexer::getPos() const
{ return new Token::Pos{pos->line, pos->column, pos->lineIndex}; }

[[noreturn]] void Lexer::fail() const
{ throw new Error{InvalidToken, string(1, current())}; }