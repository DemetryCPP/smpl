#include <map>
#include <iostream>
#include "parser.hpp"
#include "utils.hpp"

using namespace std;
using namespace SMPL::AST;

using T = Token::Name;

int tabs = 0;

void start(string n)
{ cout << string(tabs++, ' ') + "start " + n << std::endl; }

void _end(string n)
{ cout << string(--tabs, ' ') + "end   " + n << std::endl; }

Parser::Parser(Lexer *lex)
    : lex(lex)
{
    next();

    while (current->name != T::None)
    {
        cout << "huita\n";
        stmts.push_back(stmt());
    }
}

Statement *Parser::stmt()
{
    switch (current->name)
    {
        case T::Modifer: return declareStmt();
        case T::Define:  return funcStmt();
        case T::While:   return whileStmt();
        case T::If:      return ifStmt();
    }

    auto e = expr();
    match(T::Semicolon);

    return e;
}

Declare *Parser::declareStmt()
{
    auto mod = match(T::Modifer)->attribute;
    auto name = match(T::Id)->attribute;
    match(T::Assignment);
    auto value = expr();
    match(T::Semicolon);

    return new Declare(mod, name, value);
}

While *Parser::whileStmt()
{
    match(T::While);
    match(T::ORBracket);
    auto cond = expr();
    match(T::CRBracket);
    auto block = this->block();

    return new While(cond, block);
}

Func *Parser::funcStmt()
{
    match(T::Define);
    auto name = match(T::Id)->attribute;
    match(T::ORBracket);
    auto args = fargs();
    match(T::CRBracket);
    match(T::Assignment);
    auto expr = this->expr();
    match(T::Semicolon);

    return new Func(name, args, expr);
}

If *Parser::ifStmt()
{
    match(T::If);
    match(T::ORBracket);
    auto cond = expr();
    match(T::CRBracket);
    auto mblock = block();
    if (current->name == T::Else)
    {
        match(T::Else);
        auto eblock = block();
        return new If(cond, mblock, eblock);
    }
    return new If(cond, mblock);
}

Block *Parser::block()
{
    match(T::OFBracket);
    
    vector<Statement *> stmts;

    while (current->name != T::CFBracket)
        stmts.push_back(stmt());

    match(T::CFBracket);
    return new Block{stmts};
}

Expr *Parser::expr()
{ return expr(1); }

Expr *Parser::expr(size_t priority)
{
    if (priority == 1) return assign();
    if (priority == 8) return unary();
    if (priority == 9) return fact();

    vector<Expr *> nodes;
    vector<string> operators;

    nodes.push_back(expr(priority + 1));
    while (current->name == T::Operator && getPriority() == priority)
    {
        operators.push_back(match(T::Operator)->attribute);
        nodes.push_back(expr(priority + 1));
    }

    if (nodes.size() == 1)
        return nodes[0];

    return new Node(nodes, operators);
}

Expr *Parser::assign()
{
    vector<Expr *> nodes;
    vector<string> operators;

    while (following()->name == T::Assignment)
    {
        nodes.push_back(new Literal(match(T::Id)));
        match(T::Assignment);
        operators.push_back("=");
    }

    nodes.push_back(expr(2));

    if (nodes.size() == 1)
        return nodes[0];

    return new Node(nodes, operators);
}

Expr *Parser::unary()
{
    vector<string> operators;

    while (current->attribute == "!" || current->attribute == "-")
        operators.push_back(next()->attribute);
    
    if (operators.size() == 0)
        return fact();

    return new Node({ fact() }, operators);
}

Expr *Parser::fact()
{
    if (current->name == T::ORBracket)
    {
        match(T::ORBracket);
        auto expr = this->expr();
        match(T::CRBracket);
        return expr;
    }
    
    if (current->name == T::Id)
    {
        if (following()->name == T::ORBracket) return call();
        return new Literal(next());
    }

    if (current->name == T::Number)
        return new Literal(next());
}

Call *Parser::call()
{
    auto name = match(T::Id);
    match(T::ORBracket);
    auto args = cargs();
    match(T::CRBracket);

    return new Call(name, args);
}

vector<Token *> Parser::fargs()
{
    vector<Token *> result;

    if (current->name != T::Id)
        return result;

    result.push_back(match(T::Id));
    while (current->name == T::Comma)
    {
        next();
        result.push_back(match(T::Id));
    }

    return result;
}

vector<Expr *> Parser::cargs()
{
    vector<Expr *> result;

    if (current->name == T::CRBracket)
        return result;

    result.push_back(expr());
    while (current->name == T::CRBracket)
    {
        match(T::Comma);
        result.push_back(expr());
    }

    return result;
}

Token *Parser::next()
{
    if (__following != nullptr)
    {
        current = __following;
        __following = nullptr;
        return current;
    }
    
    return current = lex->next();
}

Token *Parser::following()
{
    if (__following != nullptr)
        return __following;

    return __following = lex->next();
}

Token *Parser::match(Token::Name name)
{
    if (current->name == name)
        return next();
}

size_t Parser::getPriority()
{
    if (current->attribute == "|") return 2;
    if (current->attribute == "&") return 3;
    if (current->attribute == "=="
     || current->attribute == "!=") return 4;
    if (current->attribute == ">="
     || current->attribute == "<="
     || current->attribute == ">"
     || current->attribute == "<") return 5;
    if (current->attribute == "+"
     || current->attribute == "-") return 6;
    if (current->attribute == "*"
     || current->attribute == "/") return 7;

    return 0;
}