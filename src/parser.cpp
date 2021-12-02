#include <iostream>

#include "lexer.hpp"
#include "token.hpp"
#include "parser.hpp"
#include "smpl.hpp"
#include "nodes.hpp"

AST::Parser::Parser(Lexer lex) : lex(lex) 
{
    this->next();

    while (this->current->type != Token::Type::Null)
    {
        this->stmts.push_back(this->stmt());
        this->match(";");
    }
};

void AST::Parser::next()
{ this->current = this->lex.next(); }

Token *AST::Parser::match(Token::Type type)
{
    if (this->current->type == type)
    {
        Token *result = this->current;
        this->next();
        return result;
    }
    else
        throw new SMPL::UnexpectedToken(this->lex.index, this->current->value);
}

void AST::Parser::match(std::string token)
{
    if (this->current->value == token)
        this->next();
    else
        throw new SMPL::UnexpectedToken(this->lex.index, this->current->value);
}

AST::Statement *AST::Parser::stmt()
{
    // std::cout << "stmt\n";

    if (this->current->value == "define")
        return this->function();
    else
    {
        std::string id = this->match(Token::Type::Id)->value;

        if (this->current->value == "=")        return this->assign(id);
        else if (this->current->value == "(")   return this->call(id);
    }
}

AST::Function *AST::Parser::function()
{
    // std::cout << "function\n";

    this->match("define");
    std::string name = this->match(Token::Type::Id)->value;
    this->match("(");
    std::string arg = this->match(Token::Type::Id)->value;
    this->match(")");
    this->match("=");
    AST::Expr *expr = this->expr();

    return new AST::Function(name, arg, expr);
}

AST::Assign *AST::Parser::assign(std::string id)
{
    // std::cout << "assign\n";

    this->match("=");
    AST::Expr *expr = this->expr();

    return new AST::Assign(id, expr);
}

AST::Call *AST::Parser::call(std::string id)
{
    // std::cout << "call\n";

    this->match("(");
    AST::Expr *expr = this->expr();
    this->match(")");

    return new AST::Call(id, expr);
}

AST::Expr *AST::Parser::expr()
{
    // std::cout << "expr\n";

    std::vector<AST::Term *> terms;
    std::vector<char> operators;

    terms.push_back(this->term());

    while (this->current->type == Token::Type::AOperator)
    {
        operators.push_back(this->match(Token::Type::AOperator)->value[0]);
        terms.push_back(this->term());
    }

    return new AST::Expr(terms, operators);
}

AST::Term *AST::Parser::term()
{
    // std::cout << "term\n";

    std::vector<AST::Fact *> facts;
    std::vector<char> operators;

    facts.push_back(this->fact());
    while (this->current->type == Token::Type::MOperator)
    {
        operators.push_back(this->match(Token::Type::MOperator)->value[0]);
        facts.push_back(this->fact());
    }

    return new AST::Term(facts, operators);
}

AST::Fact *AST::Parser::fact()
{
    // std::cout << "fact\n";

    if (this->current->type == Token::Type::Number)
        return new AST::Elementary(this->match(Token::Type::Number));

    else if (this->current->type == Token::Type::Id)
    {
        Token *id = this->match(Token::Type::Id);

        if (this->current->value == "(") 
            return this->call(id->value);
        
        return new AST::Elementary(id);
    }
    else if (this->current->value == "-")
    {
        this->match("-");
        AST::Fact *fact = this->fact();

        return new AST::Unary('-', fact);
    }
    else if (this->current->value == "(") 
    {
        this->match("(");
        AST::Bracket *result = new AST::Bracket(this->expr());
        this->match(")");
        return result;
    }
}