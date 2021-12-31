#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "lexer.hpp"
#include "parser.hpp"
#include "smpl.hpp"

int main(int argc, char const *argv[])
{
    std::string code, line;
    std::ifstream in("test.smpl");

    getline(in, line);
    code = line;

    while (getline(in, line))
        code += "\n" + line;

    in.close();

    auto env = new SMPL::Env;

    try
    { SMPL::eval(code, env); }
    catch (SMPL::Error *e) 
    { std::cout << e->format() << std::endl; }

    return 0;
}