#include <vector>
#include <string>

#include "token.hpp"

#pragma once

namespace AST
{
    class Fact
    {
    public:
        enum class Type
        {
            Elementary,
            Unary,
            Call,
            Brackets
        };

        Fact(Fact::Type type);

        Fact::Type type;
    };

    class Term
    {
    public:
        Term(std::vector<Fact *> facts, std::vector<char> operators);

        std::vector<Fact *> facts;
        std::vector<char> operators;
    };

    class Expr
    {
    public:
        Expr(std::vector<Term *> terms, std::vector<char> operators);

        std::vector<Term *> terms;
        std::vector<char> operators;
    };

    class Statement
    {
    public:
        enum class Type
        {
            Function,
            Assign,
            Call
        };

        Statement(Statement::Type type);

        Statement::Type type;
    };

    class Function : public Statement
    {
    public:
        Function(std::string name, std::string arg, Expr *value);

        std::string name;
        std::string arg;

        Expr *value;
    };

    class Assign : public Statement
    {
    public: 
        Assign(std::string name, Expr *value);

        std::string name;
        Expr *value;
    };

    class Call : public Fact, public Statement
    {
    public:
        Call(std::string name, Expr *expr);

        std::string name;
        Expr *expr;
    };

    class Elementary : public Fact
    {
    public:
        Elementary(Token *token);

        Token *token;
    };

    class Unary : public Fact
    {
    public:
        Unary(char oper, Fact *fact);

        char oper;
        Fact *fact;
    };

    class Bracket : public Fact
    {
    public:
        Bracket(Expr *expr);

        Expr *expr;
    };
}