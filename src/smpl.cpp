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
            case Statement::Type::Assign:   interpreter->processAssign((Assign *)stmt);     break;
            case Statement::Type::Call:     interpreter->call((Call *)stmt);                break;
            case Statement::Type::Function: interpreter->processFunction((Function *)stmt); break;
        }
    }
}

double Interpreter::solveExpr(Expr *expr)
{
    double result = solveTerm(expr->nodes[0]);
    
    for (size_t i = 1; i < expr->nodes.size(); i++)
    {
        switch (expr->operators[i - 1])
        {
            case '+': result += solveTerm(expr->nodes[i]); break;
            case '-': result -= solveTerm(expr->nodes[i]); break;
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
            case '*': result *= solveFact(term->nodes[i]); break;
            case '/': result /= solveFact(term->nodes[i]); break;
        }
    }

    return result;
}

double Interpreter::solveFact(Fact *fact)
{
    switch (fact->type)
    {
    case Fact::Type::Literal:
    {
        auto token = ((Literal *)fact)->token;

        if (token->type == Token::Type::Id)
            return get(token);

        else if (token->type == Token::Type::Number)
            return std::stod(token->value);
    }

    case Fact::Type::Brackets:  return solveExpr(((Brackets *)fact)->expr);
    case Fact::Type::Unary:     return -solveFact(((Unary *)fact)->fact);
    case Fact::Type::Call:      return call((Call *)fact);
    }
}

Function *Env::getFunction(Token *name)
{
    for (auto &&f : this->functions)
        if (f->name->value == name->value)
            return f;

    throw new Error(Error::Type::IsNotAFunction, name->line, name->column, name->value);
}

double Interpreter::get(Token *name)
{
    if (env->variables.find(name->value) != env->variables.end())
        return env->variables[name->value];

    throw new Error(Error::Type::IsNotDefined, name->line, name->column, name->value);
}

double Interpreter::call(Call *call)
{
    auto name = call->name;
    auto arg = solveExpr(call->arg);

    if (env->stdFuncs.find(name->value) != env->stdFuncs.end())
        return env->stdFuncs[name->value](arg);

    auto &variables = env->variables;
    auto func = env->getFunction(name);

    double result;

    if (variables.find(func->arg) == variables.end())
    {
        variables[func->arg] = arg;
        result = solveExpr(func->value);
        variables.erase(variables.find(func->arg));
    }
    else
    {
        auto lastValue = variables[func->arg];
        variables[func->arg] = arg;
        result = solveExpr(func->value);
        variables[func->arg] = lastValue;
    }

    return result;
}

void Interpreter::processAssign(Assign *assign)
{ env->variables[assign->name] = solveExpr(assign->value); }

void Interpreter::processFunction(Function *func)
{ env->functions.push_back(func); }

string Error::format()
{
    string result = "Error at " + to_string(line) + ":" + to_string(column) + ": ";
    string token = "\"" + this->token + "\"";

    switch (type)
    {
        case Type::UnexpectedToken: return result + "Unexpected token " + token;
        case Type::IsNotAFunction:  return result + token + " is not a function"; 
        case Type::IsNotDefined:    return result + token + " is not defined";        

        default: return "some errors..";
    }
}