#include "lexer.hpp"
#include "ast.hpp"

using namespace SMPL::AST;

class Parser
{
public:
    Parser(Lexer *lex)
        : lex(lex) {};

    Lexer *lex;
    Statement *stmt();

private:
    If *ifstatement();
    
};