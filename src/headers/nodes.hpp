#include "lexer.hpp"
#pragma once

namespace AST
{
    struct Fact
    {
        enum class Type
        {
            Brackets, Call,
            Literal, Unary,
        } type;

        Fact(Type type) : type(type) {};
    };

    struct Term
    {
        Term(std::vector<Fact *> nodes, std::vector<char> operators)
            : nodes(nodes), operators(operators) {};

        std::vector<Fact *> nodes;
        std::vector<char> operators;
    };

    struct Expr
    {
        Expr(std::vector<Term *> nodes, std::vector<char> operators)
            : nodes(nodes), operators(operators) {};

        std::vector<Term *> nodes;
        std::vector<char> operators;
    };

    struct Statement
    {
        enum class Type
        { Function, Assign, Call } type;

        Statement(Type type) : type(type) {};
    };

    struct Function : Statement
    {
        Function(Token *name, std::string arg, Expr *value)
            : Statement(Statement::Type::Function)
            , name(name), arg(arg), value(value) {};

        std::string arg;
        Token *name;
        Expr *value;
    };

    struct Assign : Statement
    {
        Assign(std::string name, Expr *value)
            : Statement(Statement::Type::Assign)
            , name(name), value(value) {};

        std::string name;
        Expr *value;
    };

    struct Call : Fact, Statement
    {
        Call(Token *name, Expr *arg)
            : Statement(Statement::Type::Call)
            , Fact(Fact::Type::Call)
            , name(name), arg(arg) {};

        Token *name;
        Expr *arg;
    };

    struct Literal : Fact
    {
        Literal(Token *token)
            : Fact(Fact::Type::Literal)
            , token(token) {};

        Token *token;
    };

    struct Unary : Fact
    {
        Unary(Fact *fact)
            : Fact(Fact::Type::Unary)
            , fact(fact) {};

        Fact *fact;
    };

    struct Brackets : Fact
    {
        Brackets(Expr *expr)
            : Fact(Fact::Type::Brackets)
            , expr(expr) {};

        Expr *expr;
    };
}