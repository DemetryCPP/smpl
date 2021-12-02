#include <string>
#include <stdexcept>
#include <iostream>

#include "smpl.hpp"
#include "token.hpp"
#include "parser.hpp"
#include "nodes.hpp"

SMPL::UnexpectedToken::UnexpectedToken(size_t index, std::string token)
    : index(index), token(token), std::logic_error("Unexpected token \"" + token + "\" at " + std::to_string(index)) {};

SMPL::UnexpectedToken::UnexpectedToken(size_t index, char token)
    : UnexpectedToken(index, std::string(1, token)) {};

SMPL::Env::Env() {};

SMPL::IsNotDefined::IsNotDefined(size_t index, std::string name)
    : name(name)
    , index(index)
    , std::logic_error("\"" + name + "\" is not defined") {};

SMPL::Interpreter::Interpreter(SMPL::Env env)
    : env(env){}

void SMPL::eval(std::string code, SMPL::Env env)
{
    AST::Parser *parser = new AST::Parser(Lexer(code));

    env.stdFuncs.insert({
        "print",
        [](double x) { std::cout << x << std::endl; return 0.0; } 
    });

    SMPL::Interpreter interpreter(env);


    for (auto &&stmt : parser->stmts)
    {
        switch (stmt->type)
        {
        case AST::Statement::Type::Assign:
        {
            AST::Assign *assign = (AST::Assign *)stmt;
            interpreter.processAssign(assign);
            break;
        }

        case AST::Statement::Type::Call:
        {
            AST::Call *call = (AST::Call *)stmt;
            interpreter.call(call);
            break;
        }

        case AST::Statement::Type::Function:
        {
            AST::Function *function = (AST::Function *)stmt;
            interpreter.processFunction(function);
            break;
        }
        }
    }
}

AST::Function *SMPL::Env::getFunction(std::string name)
{
    for (auto &&f : this->functions)
        if (f->name == name)
            return f;
}

double SMPL::Interpreter::call(AST::Call *call)
{
    std::string name = call->name;
    double arg = solveExpr(call->expr);

    if (this->env.stdFuncs.find(name) != this->env.stdFuncs.end())
        return this->env.stdFuncs[name](arg);

    std::map<std::string, double> &variables = this->env.variables;
    AST::Function *func = this->env.getFunction(name);

    double result;

    if (variables.find(func->arg) == variables.end())
    {
        variables[func->arg] = arg;
        result = this->solveExpr(func->value);          // (*)
        variables.erase(variables.find(func->arg));
    }
    else
    {
        double lastValue = variables[func->arg];
        variables[func->arg] = arg;
        result = this->solveExpr(func->value);
        variables[func->arg] = lastValue;
    }

    return result;
}

double SMPL::Interpreter::solveExpr(AST::Expr *expr)
{
    double result = solveTerm(expr->terms[0]);
    
    for (size_t i = 1; i < expr->terms.size(); i++)
    {
        switch (expr->operators[i - 1])
        {
        case '+':
            result += solveTerm(expr->terms[i]);
            break;
        
        case '-':
            result -= solveTerm(expr->terms[i]);
            break;
        }
    }

    return result;
}

double SMPL::Interpreter::solveTerm(AST::Term *term)
{
    double result = solveFact(term->facts[0]);
    
    for (size_t i = 1; i < term->facts.size(); i++)
    {
        switch (term->operators[i - 1])
        {
        case '*':
            result *= solveFact(term->facts[i]);
            break;
        
        case '/':
            result /= solveFact(term->facts[i]);
            break;
        }
    }

    return result;
}

double SMPL::Interpreter::solveFact(AST::Fact *fact)
{
    double result;

    switch (fact->type)
    {
    case AST::Fact::Type::Elementary:
    {
        Token *token = ((AST::Elementary *)fact)->token;

        if (token->type == Token::Type::Id)          
            return this->env.variables[token->value];

        else if (token->type == Token::Type::Number)
            return std::stod(token->value);
    }

    case AST::Fact::Type::Unary:
        return -solveFact(((AST::Unary *)fact)->fact);

    case AST::Fact::Type::Brackets:
        return solveExpr(((AST::Bracket *)fact)->expr);

    case AST::Fact::Type::Call:
        AST::Call *call = (AST::Call *)fact;

        if (this->env.stdFuncs.find(call->name) != this->env.stdFuncs.end())
            return this->env.stdFuncs[call->name](solveExpr(call->expr));

        return this->call(call);
    }
}

void SMPL::Interpreter::processAssign(AST::Assign *assign)
{
    this->env.variables.insert({
        assign->name,
        solveExpr(assign->value)
    });
}

void SMPL::Interpreter::processFunction(AST::Function *func)
{
    this->env.functions.push_back(func);
}