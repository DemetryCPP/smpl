#include <iostream>
#include "nodes.hpp"
#include "error.hpp"

using namespace std;
using namespace AST;
using enum Error::Type;

void Token::log() const
{
    const string names[] { 
        "[ AOperator ]",
        "[ MOperator ]",
        "[ Semicolon ]",
        "[ OpBracket ]",
        "[ ClBracket ]",
        "[  KeyWord  ]",
        "[  Assign   ]",
        "[  Number   ]",
        "[   None    ]",
        "[    Id     ]",
    };

    cout << names[(int)type] << " " << value << endl;
};

string Error::format()
{
    auto result = "Error at " + to_string(line) + ":" + to_string(column) + ": ",
         token = "\"" + this->token + "\"";

    switch (type)
    {
        case UnexpectedToken: return result + "Unexpected token " + token;
        case IsNotAFunction:  return result + token + " is not a function";
        case IsNotDefined:    return result + token + " is not defined";
        default:              return "some errors...";
    }
}

bool operator==(Token *token, Token::Type type)
{ return token->type == type; }

bool operator==(Token *token, std::string value)
{ return token->value == value; }

template<class T> T *Statement::as()
{ return static_cast<T *>(this); }

template<class T> T *Fact::as()
{ return static_cast<T *>(this); }