#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "sinrlib.h"

int N = 50, s = 5;
double e = .2;

void show(const sinr::model &model, const char *filename)
{
    std::cout << "generated" << std::endl;
    std::cout << "diameter: " << model.diameter() << std::endl;
    model.export_to_pdf(filename);
    std::cout << "exported" << std::endl;
    //model.save("model.dat");
}

int main(int argc, char **argv)
{
    std::string model_name;
    sinr::model *m;    

    if (argc < 3)
    {
        std::cout << "Usage: show_model name file\n";
        return 1;
    }
        
    //std::srand(std::time(0));
    std::srand(0);

    model_name = argv[1];

    if (model_name == "uniform")
    {
        sinr::uniform_model *tmp = new sinr::uniform_model(2.5, 1, 1 - e);
        tmp->generate(N, s);
        m = tmp;
    }
    else if (model_name == "social")
    {
        sinr::social_model *tmp = new sinr::social_model(2.5, 1, 1 - e);
        tmp->generate(N, s, e, 0.1);
        m = tmp;
    } 
    else if (model_name == "gadget")
    {
        sinr::gadget_model *tmp = new sinr::gadget_model(2.5, 1, 1 - e);
        tmp->generate(s, N, e);
        m = tmp;
    }
    else
    {
        std::cout << "unknown model name\n";
        return 2;
    }

    for (sinr::uid u = 0; u < m->get_nodes().size(); u++)
    {
        std::cout << "node " << u << ":";
        for (std::vector<sinr::uid>::const_iterator link_it = m->get_nodes()[u].links.begin();
                link_it != m->get_nodes()[u].links.end(); link_it++)
        {
            std::cout << " " << *link_it;
        }
        std::cout << '\n';
    }

    show(*m, argv[2]);

    return 0;
}

