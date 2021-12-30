#include <vector>
#include "lexer.hpp"
#include "nodes.hpp"
#pragma once

namespace AST
{
    class Parser
    {
    public:
        Parser(Lexer *lex);

        std::vector<Statement *> stmts;
        Lexer *lex;

    private:
        Statement *stmt();

        Function *function();
        Assign   *assign(std::string);
        Call     *call(std::string);

        Expr *expr();
        Term *term();
        Fact *fact();

        Brackets *brackets();
        Unary    *unary();

        Token *match(Token::Type);
        void match(std::string);
        void next();

        Token *current;
        [[noreturn]] void fail();
    };
}