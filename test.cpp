#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "parser.hpp"
#include "smpl.hpp"

using namespace SMPL;
using namespace std;

int main(int argc, char const *argv[])
{
    if (argc == 1)
        cout << "Error: no input file" << endl;

    string code, line;
    ifstream in(argv[1]);

    if (!in) cout << "Error: cannot find file " << argv[1] << endl;

    getline(in, line);
    code = line;

    while (getline(in, line))
        code += "\n" + line;

    in.close();

    auto interpreter = new Interpreter();
    interpreter->stdfuncs["print"] = [](double x) {
        cout << x << endl; return x;
    };

    try
    { interpreter->eval(code); }
    catch (Error *e) 
    { cout << e->format() << endl; }

    return 0;
}