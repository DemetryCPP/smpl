#include "nodes.hpp"
#pragma once

namespace AST
{
    using namespace std;

    class Parser
    {
    public:
        Parser(Lexer *lex);

        vector<Statement *> stmts;
        Lexer *lex;

    private:
        Statement *stmt();

        Function *function();
        Assign   *assign(string);
        Call     *call(Token *);

        Expr *expr();
        Term *term();
        Fact *fact();

        Brackets *brackets();
        Unary    *unary();

        Token *match(Token::Type);
        void match(string);
        void next();

        Token *current;
        [[noreturn]] void fail();
    };
}