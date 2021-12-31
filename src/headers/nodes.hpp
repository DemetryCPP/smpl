#include "lexer.hpp"
#pragma once

namespace AST
{
    using namespace std;

    struct Fact
    {
        enum class Type
        {
            Brackets, Call,
            Literal, Unary,
        } type;
    };

    struct Term
    {
        vector<Fact *> nodes;
        vector<char> operators;
    };

    struct Expr
    {
        vector<Term *> nodes;
        vector<char> operators;
    };

    struct Statement
    {
        enum class Type
        { Function, Assign, Call } type;
    };

    struct Function : Statement
    {
        Function(Token *name, string arg, Expr *value)
            : Statement{Statement::Type::Function}
            , name(name), arg(arg), value(value) {};

        string arg;
        Token *name;
        Expr *value;
    };

    struct Assign : Statement
    {
        Assign(string name, Expr *value)
            : Statement{Statement::Type::Assign}
            , name(name), value(value) {};

        string name;
        Expr *value;
    };

    struct Call : Fact, Statement
    {
        Call(Token *name, Expr *arg)
            : Statement{Statement::Type::Call}
            , Fact{Fact::Type::Call}
            , name(name), arg(arg) {};

        Token *name;
        Expr *arg;
    };

    struct Literal : Fact
    {
        Literal(Token *token)
            : Fact{Fact::Type::Literal}
            , token(token) {};

        Token *token;
    };

    struct Unary : Fact
    {
        Unary(Fact *fact)
            : Fact{Fact::Type::Unary}
            , fact(fact) {};

        Fact *fact;
    };

    struct Brackets : Fact
    {
        Brackets(Expr *expr)
            : Fact{Fact::Type::Brackets}
            , expr(expr) {};

        Expr *expr;
    };
}