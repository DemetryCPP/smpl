#include <stdexcept>
#include <string>
#include <map>

#include "nodes.hpp"

namespace SMPL
{
    class UnexpectedToken
    {
    public:
        UnexpectedToken(size_t index, std::string token)
            : index(index), token(token) {};
        UnexpectedToken(size_t index, char token)
            : index(index), token(std::string(1, token)) {};

        size_t index;
        std::string token;
    };

    class IsNotDefined
    {
    public:
        IsNotDefined(size_t index, std::string name)
            : index(index), name(name) {};

        size_t index;
        std::string name;
    };

    class Env
    {
    public:
        Env() {};

        using stdFunc_t = double (*)(double a);

        std::vector<AST::Function *> functions;
        std::map<std::string, stdFunc_t> stdFuncs;
        std::map<std::string, double> variables;

        AST::Function *getFunction(std::string name);
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

        Env *env;
    };

    void eval(std::string code, Env *env);
}