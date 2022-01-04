#include <vector>
#include "token.hpp"
#include "env.hpp"

namespace SMPL::AST
{
    using namespace std;

    struct Statement
    {
        enum class Type
        {
            Declare, While,
            Func, Expr, If,
        } type;
    };

    struct Expr : Statement
    {
        enum class Type
        {
            Brackets, Unary,
            Literal, Call, Node,
        } type;

        Expr(Type type) : type(type), Statement{Statement::Type::Expr} {};
    };

    struct Node : Expr
    {
        Node(vector<Expr *> nodes, vector<string> operators)
            : nodes(nodes), operators(operators), Expr(Expr::Type::Node) {};

        vector<Expr *> nodes;
        vector<string> operators;
    };

    struct Brackets : Expr
    {
        Brackets(Expr *expr) : expr(expr), Expr(Expr::Type::Brackets) {};
        Expr *expr;
    };

    struct Literal : Expr
    {
        Literal(Token *token) : token(token), Expr(Expr::Type::Literal) {};
        Token *token;
    };
    
    struct Unary : Expr
    {
        Unary(char op, Expr *expr) : expr(expr), Expr(Expr::Type::Unary) {};
        Expr *expr;
    };

    struct Call : Expr
    {
        Call(Token *name, vector<Expr *> args)
            : name(name), args(args), Expr(Expr::Type::Call) {};

        Token *name;
        vector<Expr *> args;
    };

    struct Block
    {
        Environment *env;   
        vector<Statement *> stmts;
    };

    struct Declare : Statement
    {
        Declare(string modifer, string name, Expr *value)
            : modifer(modifer), name(name), value(value)
            , Statement{Statement::Type::Declare} {};

        string modifer, name;
        Expr *value;
    };

    struct Func : Statement
    {
        Func(string name, vector<Token *> args, Expr *value)
            : name(name), args(args), value(value)
            , Statement{Statement::Type::Func} {};

        string name;
        vector<Token *> args;
        Expr *value;
    };

    struct If : Statement
    {
        If(Expr *cond, Block *block, Block *elseBlock)
            : cond(cond), block(block), elseBlock(elseBlock), hasElse(true) {};

        If(Expr *cond, Block *block)
            : cond(cond), block(block), hasElse(false)
            , Statement{Statement::Type::If} {};


        Expr *cond;
        bool hasElse;
        Block *block, *elseBlock;
    };
}