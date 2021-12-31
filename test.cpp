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
    string code, line;
    ifstream in("test.smpl");

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