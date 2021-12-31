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

    class Interpreter
    {
    public:
        using Func = double (*) (double);

        Interpreter() {};
        Interpreter(std::map<std::string, Func> stdfuncs,
                    std::map<std::string, double> variables)
            : stdfuncs(stdfuncs), variables(variables) {};

        std::map<std::string, AST::Function *> functions;
        std::map<std::string, double> variables;
        std::map<std::string, Func> stdfuncs;

        void eval(std::string code);

    private:
        double get(Token *);
        double call(AST::Call *);

        double solve(AST::Expr *);
        double solve(AST::Term *);
        double solve(AST::Fact *);

        double value(Token *);
    };
}