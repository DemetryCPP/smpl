#include <stdexcept>
#include <string>
#include <map>

#include "nodes.hpp"

namespace SMPL
{
    class UnexpectedToken : public std::logic_error
    {
    public:
        UnexpectedToken(size_t index, std::string token);
        UnexpectedToken(size_t index, char token);

        size_t index;
        std::string token;
    };

    class IsNotDefined : public std::logic_error
    {
    public:
        IsNotDefined(size_t index, std::string name);

        size_t index;
        std::string name;
    };

    class Env
    {
    public:
        Env();

        using stdFunc_t = double (*)(double a);

        std::vector<AST::Function *> functions;
        std::map<std::string, stdFunc_t> stdFuncs;
        std::map<std::string, double> variables;

        AST::Function *getFunction(std::string name);
    };

    class Interpreter
    {
    public:
        Interpreter(Env);

        void processAssign(AST::Assign *);
        void processFunction(AST::Function *);

        double solveExpr(AST::Expr *);
        double solveTerm(AST::Term *);
        double solveFact(AST::Fact *);

        double call(AST::Call *);

        Env env;
    };

    void eval(std::string code, Env env);
}