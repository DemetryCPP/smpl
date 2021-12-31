#include <map>
#include "nodes.hpp"

namespace SMPL
{
    using namespace std;
    using namespace AST;

    class Error
    {
    public:
        enum class Type
        {
            UnexpectedToken,
            IsNotAFunction,
            IsNotDefined,
        } type;

        Error(Type type, size_t line, size_t column, string token) 
            : line(line), column(column), token(token), type(type) {};

        Error(Type type, size_t line, size_t column, char token)
            : Error(type, line, column, string(1, token)) {};

        size_t line, column;
        string token;

        string format();
    };

    class Interpreter
    {
    public:
        using Func = double (*) (double);

        Interpreter() {};
        Interpreter(map<string, Func> stdfuncs,
                    map<string, double> variables)
            : stdfuncs(stdfuncs), variables(variables) {};

        map<string, Function *> functions;
        map<string, double> variables;
        map<string, Func> stdfuncs;

        void eval(string code);

    private:
        double get(Token *);
        double call(Call *);

        double solve(Expr *);
        double solve(Term *);
        double solve(Fact *);

        double value(Token *);
    };
}