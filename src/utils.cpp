#include <iostream>
#include "nodes.hpp"
#include "error.hpp"

using namespace std;
using namespace AST;
using enum Error::Type;

#define COLOR_RED     "\x1b[31m" "\x1b[1m"
#define COLOR_RESET   "\x1b[0m"

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

string Error::format(const string &code, const string &filename)
{
    auto position = filename + ":" + to_string(pos->line) + ":" + to_string(pos->column) + ": " COLOR_RED "Error: " COLOR_RESET;
    auto token = "\"" + this->token + "\"";
    string text;

    switch (type)
    {
        case UnexpectedToken: text = "Unexpected token " + token;   break;
        case IsNotAFunction:  text = token + " is not a function";  break;
        case IsNotDefined:    text = token + " is not defined";     break;
        default:              text = "some errors...";              break;
    }

    auto result = position + text + "\n";
    auto lineNum = to_string(pos->line);
    auto prefix = string(3 - lineNum.length(), ' ') + lineNum + " | ";
    string line;

    for (auto i = pos->lineIndex; code[i] != '\n' && i < code.length(); i++)
    {
        if (i - pos->lineIndex + 1 == pos->column)
            line += COLOR_RED;
        if (i - pos->lineIndex + 1 == pos->column + this->token.length())
            line += COLOR_RESET;
            
        line += code[i];
    }

    line += COLOR_RESET;

    result += prefix + line + '\n';
    result += "    | " + string(pos->column - 1, ' ') + COLOR_RED "^" + string(this->token.length() - 1, '~') + COLOR_RESET;

    return result;
}

bool operator==(Token *token, Token::Type type)
{ return token->type == type; }

bool operator==(Token *token, std::string value)
{ return token->value == value; }