#include <iostream>
#include <map>
#include "parser.hpp"
#include "smpl.hpp"

using namespace SMPL;
using namespace std;
using namespace AST;

using enum Token::Type;
using enum Error::Type;

Parser::Parser(Lexer *lex) : lex(lex) 
{
    next();

    while (current != None)
    {
        try
        { 
            stmts.push_back(stmt());
            match(Semicolon);
        }
        catch (Error *e)
        { errors.push_back(e); }
    }
};

Statement *Parser::stmt()
{
    if (current == Define) return function();
    else
    {
        auto id = match(Id);

        if (current == Assignment) return assign(id->value);
        if (current == OBracket)   return call(id);
    }

    fail("statement");
}

Function *Parser::function()
{
    match(Define);
    auto name = match(Id);
    match(OBracket);
    auto arg = match(Id)->value;
    match(CBracket);
    match(Assignment);

    return new Function(name, arg, expr());
}

Assign *Parser::assign(string id)
{
    match(Assignment);
    return new Assign(id, expr());
}

Call *Parser::call(Token *id)
{
    match(OBracket);
    auto expr = this->expr();
    match(CBracket);

    return new Call(id, expr);
}

Expr *Parser::expr()
{
    vector<Term *> terms;
    vector<char> operators;

    terms.push_back(term());

    while (current == AOperator)
    {
        operators.push_back(match(AOperator)->value[0]);
        terms.push_back(term());
    }

    return new Expr{terms, operators};
}

Term *Parser::term()
{
    vector<Fact *> facts;
    vector<char> operators;

    facts.push_back(fact());
    while (current == MOperator)
    {
        operators.push_back(match(MOperator)->value[0]);
        facts.push_back(fact());
    }

    return new Term{facts, operators};
}

Fact *Parser::fact()
{
    if (current == Number)
        return new Literal(match(Number));

    else if (current == Id)
    {
        auto id = match(Id);

        if (current == OBracket) 
            return call(id);
        
        return new Literal(id);
    }
    else if (current == AOperator) return unary();
    else if (current == OBracket)  return brackets();

    fail("expression");
}

Brackets *Parser::brackets()
{
    match(OBracket);
    auto expr = this->expr();
    match(CBracket);

    return new Brackets(expr);
}

Unary *Parser::unary()
{ return new Unary(match(AOperator)->value[0], fact()); }

Token *Parser::match(Token::Type type)
{
    if (current == type)
    {
        auto result = current;
        next();
        return result;
    }
    
    fail(map<Token::Type, string>({
        { Assignment, "'='"         }, 
        { AOperator,  "operator"    },
        { MOperator,  "operator"    },
        { Semicolon,  "';'"         },
        { OBracket,   "'('"         },
        { CBracket,   "')'"         },
        { Number,     "number"      },
        { Id,         "identifier"  }
    })[type]);
}

void Parser::match(string token)
{
    if (current->value == token) next();
    else fail(token);
}

void Parser::next()
{ current = lex->next(); }

void Parser::fail(string expected)
{
    auto e = new Error(UnexpectedToken, current->pos, current->value, "expected " + expected);

    while (current != Semicolon) next();
    match(Semicolon);

    throw e;
}