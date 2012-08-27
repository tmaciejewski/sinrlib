#include <string>
#include <iostream>
#include <cmath>

#include "sinrlib.h"

int N = 20, s = 1;
double e = .2;

void show(const sinr::model &model, const char *filename)
{
    std::cout << "generated" << std::endl;
    std::cout << "diameter: " << model.diameter() << std::endl;
    model.export_to_pdf(s, filename);
    std::cout << "exported" << std::endl;
    //model.save("model.dat");
}

int main(int argc, char **argv)
{
    sinr::config conf;
    std::string model_name;
        
    if (argc < 3)
    {
        std::cout << "Usage: show_model name file\n";
        return 1;
    }
        
    model_name = argv[1];

    if (model_name == "uniform")
    {
        sinr::uniform_model model(conf, N, s, 1 - e);
        //sinr::model model(conf);
        //model.load("model.dat");
        show(model, argv[2]);
    }
    //elif sys.argv[1] == 'social':
    //    model = sinrlib.SocialModel(config, N, s, e, .1, 1 - e)
    //elif sys.argv[1] == 'gadget':
    //    model = sinrlib.GadgetModel(config, 10, 5, 0.1)
    else
    {
        std::cout << "unknown model name\n";
        return 2;
    }

    return 0;
}

