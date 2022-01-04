#include <string>
#pragma once

class Token
{
public:
    enum class Name
    {
        Undefined,
        None,

        // keywords
        Modifer,
        Define,
        While,
        Else,
        If,

        // special chars
        Assignment,
        Semicolon,
        ORBracket,
        CRBracket,
        OFBracket,
        CFBracket,
        Comma,

        // all
        Operator,
        Number,
        Id,
    } name;

    struct Pos
    { size_t line, column, lineIndex; } *pos;

    Token(Name name, Pos *pos)
        : Token(name, pos, "") {};

    Token(Name name, Pos *pos, char attribute)
        : name(name), pos(pos), attribute(std::string(1, attribute)) {};

    Token(Name name, Pos *pos, std::string attribute)
        : name(name), pos(pos), attribute(attribute) {};

    std::string attribute;
    void log() const;
};