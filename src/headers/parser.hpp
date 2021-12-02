#include <vector>

#include "lexer.hpp"
#include "token.hpp"
#include "nodes.hpp"

#pragma once

namespace AST
{
    class Parser
    {
    private:
        Token *current;

        void next();
        void match(std::string);

        Token *match(Token::Type);

        Statement *stmt();

        Function *function();
        Assign *assign(std::string);
        Call *call(std::string);

        Expr *expr();
        Term *term();
        Fact *fact();

    public:
        Parser(Lexer lex);

        std::vector<Statement *> stmts;

        Lexer lex;
    };
}