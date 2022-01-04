#include "data.hpp"

namespace SMPL
{
    using namespace std;

    struct Environment
    {
        vector<Data::Variable *> variables;
        vector<Data::Function *> functions;        

        Environment *parent;
    };
}