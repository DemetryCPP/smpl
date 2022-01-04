#include <iostream>
#include <map>
#include "token.hpp"

using enum Token::Name;

void Token::log() const
{
    auto name = std::map<Token::Name, std::string>({
        { Define, "define" },
        { While,  "while"  },
        { Else,   "else"   },
        { If,     "if"     },

        { Assignment, "=" },
        { Semicolon,  ";" },
        { ORBracket,  "(" },
        { CRBracket,  ")" },
        { OFBracket,  "{" },
        { CFBracket,  "}" },
        { Comma,      "," },
    })[this->name];

    if (!name.empty())
        std::cout << "{" + name + "}" << std::endl;

    else 
    {
        name = std::map<Token::Name, std::string>({
            { Operator, "operator" },
            { Number,   "number"   },
            { Id,       "id"       },
        })[this->name];

        std::cout << "{" + name + ", " + attribute + "}" << std::endl;
    }
}