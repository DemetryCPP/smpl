#include <vector>
#include <string>
#include <iostream>

#include "token.hpp"
#include "nodes.hpp"

AST::Statement::Statement(AST::Statement::Type type) : type(type) {};
AST::Fact::Fact(Fact::Type type) : type(type) {};

AST::Function::Function(std::string name, std::string arg, AST::Expr *value)
    : name(name)
    , arg(arg)
    , value(value)
    , AST::Statement(AST::Statement::Type::Function) 
{};

AST::Assign::Assign(std::string name, Expr *value)
    : name(name)
    , value(value)
    , AST::Statement(AST::Statement::Type::Assign) 
{};

AST::Call::Call(std::string name, AST::Expr *expr)
    : name(name)
    , expr(expr)
    , AST::Statement(AST::Statement::Type::Call)
    , AST::Fact(AST::Fact::Type::Call)
{};

AST::Expr::Expr(std::vector<AST::Term *> terms, std::vector<char> operators)
    : terms(terms)
    , operators(operators) {};

AST::Term::Term(std::vector<AST::Fact *> facts, std::vector<char> operators)
    : facts(facts)
    , operators(operators) {};

AST::Elementary::Elementary(Token *token)
    : token(token)
    , AST::Fact(AST::Fact::Type::Elementary){};

AST::Unary::Unary(char oper, Fact *fact)
    : oper(oper)
    , fact(fact)
    , AST::Fact(AST::Fact::Type::Unary){};

AST::Bracket::Bracket(Expr *expr)
    : expr(expr)
    , AST::Fact(AST::Fact::Type::Brackets){};