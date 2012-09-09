#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "sinrlib.h"

int N = 50, s = 1;
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
        tmp->generate(s, N);
        m = tmp;
    }
    else
    {
        std::cout << "unknown model name\n";
        return 2;
    }

    for (std::map<sinr::uid, std::vector<sinr::uid> >::const_iterator node_it = m->get_links().begin();
            node_it != m->get_links().end(); node_it++)
    {
        std::cout << "node " << node_it->first << ":";
        for (std::vector<sinr::uid>::const_iterator link_it = node_it->second.begin();
                link_it != node_it->second.end(); link_it++)
        {
            std::cout << " " << *link_it;
        }
        std::cout << '\n';
    }

    show(*m, argv[2]);

    return 0;
}

