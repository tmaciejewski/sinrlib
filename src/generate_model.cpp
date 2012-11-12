#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "sinrlib.h"

int main(int argc, char **argv)
{
    std::string model_name;
    sinr::model *m;
    int N, s;

    if (argc < 5)
    {
        std::cout << "Usage: " << argv[0] << " name N s file\n";
        return 1;
    }
        
    std::srand(std::time(0));
    //std::srand(0);

    model_name = argv[1];
    std::istringstream(argv[2]) >> N;
    std::istringstream(argv[3]) >> s;

    if (model_name == "uniform")
    {
        sinr::uniform_model *tmp = new sinr::uniform_model();
        tmp->generate(N, s);
        m = tmp;
    }
    else if (model_name == "social")
    {
        sinr::social_model *tmp = new sinr::social_model();
        tmp->generate(N, s, 0.2, 0.2);
        m = tmp;
    } 
    else if (model_name == "gadget")
    {
        sinr::gadget_model *tmp = new sinr::gadget_model();
        tmp->generate(s, N, 0.1);
        m = tmp;
    }
    else
    {
        std::cout << "unknown model name\n";
        return 2;
    }

    m->save(argv[4]);

    return 0;
}

