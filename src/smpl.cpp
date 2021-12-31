#include <iostream>
#include "smpl.hpp"
#include "parser.hpp"

using namespace SMPL;
using namespace AST;
using namespace std;

void Interpreter::eval(string code)
{
    auto parser = new Parser(new Lexer(code));

    for (auto &&stmt : parser->stmts)
    {
        switch (stmt->type)
        {
        case Statement::Type::Assign: {
            auto assign = static_cast<Assign *>(stmt);
            variables[assign->name] = solve(assign->value);
        } break;

        case Statement::Type::Call: {
            call(static_cast<Call *>(stmt));
        } break;
    
        case Statement::Type::Function: {
            auto function = static_cast<Function *>(stmt);
            functions[function->name->value] = function;
        } break;
        }
    }
}

double Interpreter::solve(Expr *expr)
{
    double result = solve(expr->nodes[0]);
    
    for (size_t i = 1; i < expr->nodes.size(); i++)
    {
        switch (expr->operators[i - 1])
        {
            case '+': result += solve(expr->nodes[i]); break;
            case '-': result -= solve(expr->nodes[i]); break;
        }
    }

    return result;
}

double Interpreter::solve(Term *term)
{
    double result = solve(term->nodes[0]);
    
    for (size_t i = 1; i < term->nodes.size(); i++)
    {
        switch (term->operators[i - 1])
        {
            case '*': result *= solve(term->nodes[i]); break;
            case '/': result /= solve(term->nodes[i]); break;
        }
    }

    return result;
}

double Interpreter::solve(Fact *fact)
{
    switch (fact->type)
    {
        case Fact::Type::Literal:  return  value(static_cast<Literal  *>(fact)->token);
        case Fact::Type::Brackets: return  solve(static_cast<Brackets *>(fact)->expr);
        case Fact::Type::Unary:    return -solve(static_cast<Unary    *>(fact)->fact);
        case Fact::Type::Call:     return  call( static_cast<Call     *>(fact));
    }
}

double Interpreter::get(Token *name)
{
    if (variables.find(name->value) != variables.end())
        return variables[name->value];

    throw new Error(Error::Type::IsNotDefined, name->line, name->column, name->value);
}

double Interpreter::call(Call *call)
{
    auto name = call->name;
    auto arg = solve(call->arg);

    if (stdfuncs.find(name->value) != stdfuncs.end())
        return stdfuncs[name->value](arg);

    if (functions.find(name->value) == functions.end())
        throw new Error(Error::Type::IsNotAFunction, name->line, name->column, name->value);

    auto func = functions[name->value];
    double result;

    if (variables.find(func->arg) == variables.end())
    {
        variables[func->arg] = arg;
        result = solve(func->value);
        variables.erase(variables.find(func->arg));
    }
    else
    {
        auto lastValue = variables[func->arg];
        variables[func->arg] = arg;
        result = solve(func->value);
        variables[func->arg] = lastValue;
    }

    return result;
}

double Interpreter::value(Token *token)
{
    if (token->type == Token::Type::Number)
        return stod(token->value);

    if (token->type == Token::Type::Id)
        return get(token);
}

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