#include <iostream>
#include "smpl.hpp"
#include "parser.hpp"

using namespace SMPL;
using namespace AST;
using namespace std;
using enum Error::Type;

using SType = Statement::Type;
using FType = Fact::Type;

void Interpreter::eval(string code)
{
    auto parser = new Parser(new Lexer(code));

    for (auto &&stmt : parser->stmts)
    {
        switch (stmt->type)
        {
        case SType::Assign: {
            auto assign = static_cast<Assign *>(stmt);
            variables[assign->name] = solve(assign->value);
        } break;

        case SType::Call: {
            call(static_cast<Call *>(stmt));
        } break;
    
        case SType::Function: {
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
        case FType::Brackets: return  solve(static_cast<Brackets *>(fact)->expr);
        case FType::Literal:  return  value(static_cast<Literal  *>(fact)->token);
        case FType::Unary:    return -solve(static_cast<Unary    *>(fact)->fact);
        case FType::Call:     return   call(static_cast<Call     *>(fact));
    }
}

template<class K, class V> bool includes(map<K, V> m, K key)
{ return m.find(key) != m.end(); }

double Interpreter::get(Token *name)
{
    if (includes<string, double>(variables, name->value))
        return variables[name->value];

    throw new Error(IsNotDefined, name->line, name->column, name->value);
}

double Interpreter::call(Call *call)
{
    auto name = call->name;
    auto arg = solve(call->arg);

    if (includes<string, Func>(stdfuncs, name->value))
        return stdfuncs[name->value](arg);

    if (!includes<string, Function *>(functions, name->value))
        throw new Error(IsNotAFunction, name->line, name->column, name->value);

    auto argOverlapGlobals = includes<string, double>(variables, name->value);
    auto func = functions[name->value];
    auto argname = func->arg;
    double result, temp;

    if (argOverlapGlobals)
        temp = variables[argname];

    variables[argname] = arg;
    result = solve(func->value);

    if (argOverlapGlobals)
        variables[argname] = temp;
    else
        variables.erase(variables.find(argname));

    return result;
}

double Interpreter::value(Token *token)
{
    if (token->type == Token::Type::Number) return stod(token->value);
    if (token->type == Token::Type::Id)     return get(token);
}

string Error::format()
{
    string result = "Error at " + to_string(line) + ":" + to_string(column) + ": ";
    string token = "\"" + this->token + "\"";

    switch (type)
    {
        case UnexpectedToken: return result + "Unexpected token " + token;
        case IsNotAFunction:  return result + token + " is not a function"; 
        case IsNotDefined:    return result + token + " is not defined";        

        default: return "some errors..";
    }
}