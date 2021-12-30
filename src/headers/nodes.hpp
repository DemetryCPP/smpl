#include "token.hpp"
#pragma once

namespace AST
{
    class Fact
    {
    public:
        enum class Type
        {
            Brackets, Call,
            Literal, Unary,
        } type;

        Fact(Type type) : type(type) {};
    };

    class Term
    {
    public:
        Term(std::vector<Fact *> nodes, std::vector<char> operators)
            : nodes(nodes), operators(operators) {};

        std::vector<Fact *> nodes;
        std::vector<char> operators;
    };

    class Expr
    {
    public:
        Expr(std::vector<Term *> nodes, std::vector<char> operators)
            : nodes(nodes), operators(operators) {};

        std::vector<Term *> nodes;
        std::vector<char> operators;
    };

    class Statement
    {
    public:
        enum class Type
        { Function, Assign, Call } type;

        Statement(Type type) : type(type) {};
    };

    class Function : public Statement
    {
    public:
        Function(std::string name, std::string arg, Expr *value)
            : Statement(Statement::Type::Function)
            , name(name), arg(arg), value(value) {};

        std::string name;
        std::string arg;

        Expr *value;
    };

    class Assign : public Statement
    {
    public: 
        Assign(std::string name, Expr *value)
            : Statement(Statement::Type::Assign)
            , name(name), value(value) {};

        std::string name;
        Expr *value;
    };

    class Call : public Fact, public Statement
    {
    public:
        Call(std::string name, Expr *expr)
            : Statement(Statement::Type::Call)
            , Fact(Fact::Type::Call)
            , name(name), expr(expr) {};

        std::string name;
        Expr *expr;
    };

    class Literal : public Fact
    {
    public:
        Literal(Token *token)
            : Fact(Fact::Type::Literal)
            , token(token) {};

        Token *token;
    };

    class Unary : public Fact
    {
    public:
        Unary(char oper, Fact *fact)
            : Fact(Fact::Type::Unary)
            , oper(oper), fact(fact) {};

        char oper;
        Fact *fact;
    };

    class Brackets : public Fact
    {
    public:
        Brackets(Expr *expr)
            : Fact(Fact::Type::Brackets)
            , expr(expr) {};

        Expr *expr;
    };
}