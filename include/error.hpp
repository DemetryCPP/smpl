#include <string>
#pragma once

namespace SMPL
{
    struct Error
    {
        enum class Type
        {
            InvalidToken
        } type;

        std::string value;
    };
}