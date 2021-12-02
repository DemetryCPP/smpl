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

    while (getline(in, line))
        code += "\n" + line;

    in.close();

    SMPL::Env env{};

    SMPL::eval(code, env);

    return 0;
}