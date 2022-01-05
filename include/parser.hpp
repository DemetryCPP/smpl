#include "lexer.hpp"
#include "data.hpp"
#include "ast.hpp"

namespace SMPL::AST
{
    class Parser
    {
    public:
        Parser(Lexer *lex);

        Lexer *lex;
        Statement *stmt();

        vector<Statement *> stmts;

    private:
        Declare *declareStmt();
        While   *whileStmt();
        Func    *funcStmt();
        If      *ifStmt();

        Block *block();

        Expr *expr();
        Expr *expr(size_t);

        Expr *assign();
        Expr *unary();
        Expr *fact();
        Call *call();

        vector<Token *> fargs();
        vector<Expr *> cargs();

        Token *current, *__following = nullptr;

        Token *following();
        Token *match(Token::Name);
        Token *next();

        size_t getPriority();
    };
}