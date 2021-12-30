#include <iostream>
#include "parser.hpp"
#include "smpl.hpp"

using namespace SMPL;
using namespace std;
using namespace AST;

Parser::Parser(Lexer *lex) : lex(lex) 
{
    next();

    while (current->type != Token::Type::Null)
    {
        stmts.push_back(stmt());
        match(";");
    }
};

Statement *Parser::stmt()
{
    if (current->value == "define") return function();
    else
    {
        string id = match(Token::Type::Id)->value;

        if (current->value == "=")      return assign(id);
        else if (current->value == "(") return call(id);
    }

    fail();
}

Function *Parser::function()
{
    match("define");
    auto name = match(Token::Type::Id)->value;
    match("(");
    auto arg = match(Token::Type::Id)->value;
    match(")");
    match("=");
    auto expr = this->expr();

    return new Function(name, arg, expr);
}

Assign *Parser::assign(string id)
{
    match("=");
    auto expr = this->expr();

    return new Assign(id, expr);
}

Call *Parser::call(string id)
{
    match("(");
    auto expr = this->expr();
    match(")");

    return new Call(id, expr);
}

Expr *Parser::expr()
{
    vector<AST::Term *> terms;
    vector<char> operators;

    terms.push_back(term());

    while (current->type == Token::Type::AOperator)
    {
        operators.push_back(match(Token::Type::AOperator)->value[0]);
        terms.push_back(term());
    }

    return new Expr(terms, operators);
}

Term *Parser::term()
{
    vector<Fact *> facts;
    vector<char> operators;

    facts.push_back(fact());
    while (current->type == Token::Type::MOperator)
    {
        operators.push_back(match(Token::Type::MOperator)->value[0]);
        facts.push_back(fact());
    }

    return new Term(facts, operators);
}

Fact *Parser::fact()
{
    if (current->type == Token::Type::Number)
        return new Literal(match(Token::Type::Number));

    else if (current->type == Token::Type::Id)
    {
        Token *id = match(Token::Type::Id);

        if (current->value == "(") 
            return call(id->value);
        
        return new Literal(id);
    }
    else if (current->value == "-") return unary();
    else if (current->value == "(") return brackets();

    fail();
}

Brackets *Parser::brackets()
{
    match("(");
    auto expr = this->expr();
    match(")");

    return new Brackets(expr);
}

Unary *Parser::unary()
{
    match("-");
    return new Unary('-', fact());
}

Token *Parser::match(Token::Type type)
{
    if (current->type == type)
    {
        Token *result = this->current;
        next();
        return result;
    }
    
    fail();
}

void Parser::match(std::string token)
{
    if (current->value == token) next();
    else throw new SMPL::UnexpectedToken(lex->index, current->value);
}

void Parser::next()
{ current = lex->next(); }

[[noreturn]] void Parser::fail()
{ throw new UnexpectedToken(lex->index, current->value); }