#include <map>
#include "nodes.hpp"

namespace SMPL
{
    class Error
    {
    public:
        enum class Type
        {
            UnexpectedToken,
            IsNotAFunction,
            IsNotDefined,
        } type;

        Error(Type type, size_t line, size_t column, std::string token) 
            : line(line), column(column), token(token), type(type) {};

        Error(Type type, size_t line, size_t column, char token)
            : Error(type, line, column, std::string(1, token)) {};

        size_t line, column;
        std::string token;

        std::string format();
    };

    class Env
    {
    public:
        Env() {};

        using stdFunc_t = double (*)(double a);

        std::vector<AST::Function *> functions;
        std::map<std::string, stdFunc_t> stdFuncs;
        std::map<std::string, double> variables;

        AST::Function *getFunction(Token *name);
    };

    class Interpreter
    {
    public:
        Interpreter(Env *env) : env(env) {};

        void processAssign(AST::Assign *);
        void processFunction(AST::Function *);

        double solveExpr(AST::Expr *);
        double solveTerm(AST::Term *);
        double solveFact(AST::Fact *);

        double call(AST::Call *);
        double get(Token *);

        Env *env;
    };

    void eval(std::string code, Env *env);
}