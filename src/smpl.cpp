#include <iostream>
#include "smpl.hpp"
#include "parser.hpp"

using namespace SMPL;
using namespace AST;
using namespace std;

void SMPL::eval(string code, Env *env)
{
    auto parser = new Parser(new Lexer(code));
    auto interpreter = new Interpreter(env);

    env->stdFuncs["print"] = [](double x) { std::cout << x << std::endl; return x; };

    for (auto &&stmt : parser->stmts)
    {
        switch (stmt->type)
        {
        case Statement::Type::Assign:
            interpreter->processAssign((Assign *)stmt);
            break;

        case Statement::Type::Call:
            interpreter->call((Call *)stmt);
            break;

        case Statement::Type::Function:
            interpreter->processFunction((Function *)stmt);
            break;
        }
    }
}

Function *Env::getFunction(string name)
{
    for (auto &&f : this->functions)
        if (f->name == name)
            return f;
}

double Interpreter::call(Call *call)
{
    string name = call->name;
    double arg = solveExpr(call->expr);

    if (env->stdFuncs.find(name) != env->stdFuncs.end())
        return env->stdFuncs[name](arg);

    map<string, double> &variables = env->variables;
    Function *func = env->getFunction(name);

    double result;

    if (variables.find(func->arg) == variables.end())
    {
        variables[func->arg] = arg;
        result = solveExpr(func->value);
        variables.erase(variables.find(func->arg));
    }
    else
    {
        double lastValue = variables[func->arg];
        variables[func->arg] = arg;
        result = solveExpr(func->value);
        variables[func->arg] = lastValue;
    }

    return result;
}

double Interpreter::solveExpr(Expr *expr)
{
    double result = solveTerm(expr->nodes[0]);
    
    for (size_t i = 1; i < expr->nodes.size(); i++)
    {
        switch (expr->operators[i - 1])
        {
        case '+':
            result += solveTerm(expr->nodes[i]);
            break;
        
        case '-':
            result -= solveTerm(expr->nodes[i]);
            break;
        }
    }

    return result;
}

double Interpreter::solveTerm(Term *term)
{
    double result = solveFact(term->nodes[0]);
    
    for (size_t i = 1; i < term->nodes.size(); i++)
    {
        switch (term->operators[i - 1])
        {
        case '*':
            result *= solveFact(term->nodes[i]);
            break;
        
        case '/':
            result /= solveFact(term->nodes[i]);
            break;
        }
    }

    return result;
}

double Interpreter::solveFact(Fact *fact)
{
    double result;

    switch (fact->type)
    {
    case Fact::Type::Literal:
    {
        auto token = ((Literal *)fact)->token;

        if (token->type == Token::Type::Id)          
            return env->variables[token->value];

        else if (token->type == Token::Type::Number)
            return std::stod(token->value);
    }

    case Fact::Type::Unary:
        return -solveFact(((Unary *)fact)->fact);

    case Fact::Type::Brackets:
        return solveExpr(((Brackets *)fact)->expr);

    case Fact::Type::Call:
        return call((Call *)fact);
    }
}

void Interpreter::processAssign(Assign *assign)
{ env->variables[assign->name] = solveExpr(assign->value); }

void Interpreter::processFunction(Function *func)
{ env->functions.push_back(func); }