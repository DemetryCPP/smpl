#include <string>
#include <vector>
#pragma once

namespace SMPL::Data
{
    struct Operator
    {
        using Action = double (*)(double a, double b);

        std::string name;
        size_t priority;
        enum class Type
        { Binary, Unary } type;

        Action action;
    };

    struct Variable
    {
        std::string name;
        bool isConst;
    };

    struct Function
    {
        using Action = double (*)(std::vector<double>);

        std::string name;
        size_t argCount;
        Action action;
    };
}