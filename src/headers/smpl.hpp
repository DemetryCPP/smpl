#include <map>
#include "nodes.hpp"
#include "error.hpp"

namespace SMPL
{
    using namespace std;
    using namespace AST;
    
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
        vector<string> definedFunctions;
        vector<string> definedVariables;

        void checkSemantic(vector<Statement *>);
        void checkExprSemantic(Expr *);
        void checkCallSemantic(Call *);
        void checkFactSemantic(Fact *);

        double get(Token *);
        double call(Call *);

        double solve(Expr *);
        double solve(Term *);
        double solve(Fact *);

        double value(Token *);

        vector<Error *> errors;
    };

    inline const string version = "1.1";
}