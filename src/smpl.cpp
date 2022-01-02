#include <algorithm>
#include "smpl.hpp"
#include "parser.hpp"

using namespace SMPL;
using namespace AST;
using namespace std;

using enum Error::Type;
using enum Token::Type;

using SType = Statement::Type;
using FType = Fact::Type;

void Interpreter::eval(string code)
{
    auto parser = new Parser(new Lexer(code));
    errors = parser->errors;

    checkSemantic(parser->stmts);

    if (errors.size() != 0)
        throw errors;

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
            checkExprSemantic(function->value);
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
        case FType::Brackets: return solve(fact->as<Brackets>()->expr);
        case FType::Literal:  return value(fact->as<Literal >()->token);
        case FType::Unary:    return solve(fact->as<Unary   >()->fact) 
            * (fact->as<Unary>()->op == '-' ? -1 : 1);
        case FType::Call:     return  call(fact->as<Call    >());
    }
}

#define enlist(x) std::begin(x), std::end(x)

template<class K, class V> bool includes(const map<K, V> &m, K key)
{ return m.find(key) != m.end(); }

template<class T> bool includes(const vector<T> &v, T val)
{ return std::find(enlist(v), val) != v.end(); }

double Interpreter::get(Token *name)
{
    if (includes<string, double>(variables, name->value))
        return variables[name->value];

    throw new Error(IsNotDefined, name->pos, name->value);
}

double Interpreter::call(Call *call)
{
    auto name = call->name;
    auto arg = solve(call->arg);

    if (includes<string, Func>(stdfuncs, name->value))
        return stdfuncs[name->value](arg);

    auto func = functions[name->value];
    auto argname = func->arg;
    auto argOverlapGlobals = includes<string, double>(variables, argname);
    double result, temp;

    if (argOverlapGlobals)
        temp = variables[argname];

    variables[argname] = arg;
    result = solve(func->value);

    if (argOverlapGlobals) variables[argname] = temp;
    else variables.erase(variables.find(argname));

    return result;
}

double Interpreter::value(Token *token)
{
    if (token == Number) return stod(token->value);
    if (token == Id)     return get(token);
}

void Interpreter::checkSemantic(vector<Statement *> stmts)
{
    for (auto &&stmt : stmts)
    {
        switch (stmt->type)
        {
        case SType::Function: {
            checkFunctionSemantic(static_cast<Function *>(stmt));
        } break;

        case SType::Assign: {
            auto assign = static_cast<Assign *>(stmt);
            definedVariables.push_back(assign->name);
            checkExprSemantic(assign->value);
        } break;

        case SType::Call: {
            checkCallSemantic(static_cast<Call *>(stmt));
        } break;
        }
    }
}

void Interpreter::checkFunctionSemantic(Function *function)
{
    auto argOverlapGlobals = includes<string, double>(variables, function->arg);
    definedVariables.push_back(function->arg);
    definedFunctions.push_back(function->name->value);
    checkExprSemantic(function->value);
    if (!argOverlapGlobals)
        definedVariables.pop_back();
}

void Interpreter::checkCallSemantic(Call *call)
{
    if (!includes<string>(definedFunctions, call->name->value) && !includes<string, Func>(stdfuncs, call->name->value))
        errors.push_back(new Error(IsNotAFunction, call->name->pos, call->name->value));
    checkExprSemantic(call->arg);
}

void Interpreter::checkExprSemantic(Expr *expr)
{
    for (auto &&t : expr->nodes)
        for (auto &&f : t->nodes)
            checkFactSemantic(f);
}

void Interpreter::checkFactSemantic(Fact *f)
{
    switch (f->type)
    {
    case FType::Brackets:
        checkExprSemantic(f->as<Brackets>()->expr);
        break;
    
    case FType::Literal: {
        auto literal = f->as<Literal>()->token;
        if (literal->type == Id)
            if (!includes(definedVariables, literal->value) && !includes(variables, literal->value))
                errors.push_back(new Error(IsNotDefined, literal->pos, literal->value));
    } break;

    case FType::Unary: {
        checkFactSemantic(f->as<Unary>()->fact);
    } break;

    case FType::Call: {
        checkCallSemantic(f->as<Call>());
    } break;
    }
}