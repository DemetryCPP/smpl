#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "parser.hpp"
#include "smpl.hpp"

using namespace SMPL;
using namespace std;

int comp(Error *a, Error *b)
{
    return a->pos->line - b->pos->line;
};

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        cout << "Error: no input file" << endl;
        return 1;
    }

    string code, line;
    ifstream in(argv[1]);

    if (!in) cout << "Error: cannot find file " << argv[1] << endl;

    getline(in, line);
    code = line;

    while (getline(in, line))
        code += "\n" + line;

    in.close();

    auto interpreter = new Interpreter();
    interpreter->stdfuncs["print"] = [](double x) { cout << x << endl; return x; };
    interpreter->stdfuncs["input"] = [](double x) { cin >> x; return x; };

    try
    { interpreter->eval(code); }
    catch (vector<Error *> errors)
    {
        sort(errors.begin(), errors.end(), comp);
        for (auto &&e : errors)
            cout << e->format(code, argv[1]) << endl;
    }

    return 0;
}