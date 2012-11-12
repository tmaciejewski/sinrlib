#include <string>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "sinrlib.h"

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
    double e = 0.2;
    sinr::model m(2.5, 1, 1 - e);    

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " input output\n";
        return 1;
    }

    m.load(argv[1]);
        
    for (sinr::uid u = 0; u < m.get_nodes().size(); u++)
    {
        //std::cout << "node " << u << ":";
        for (std::vector<sinr::uid>::const_iterator link_it = m.get_nodes()[u].links.begin();
                link_it != m.get_nodes()[u].links.end(); link_it++)
        {
            //std::cout << " " << *link_it;
        }
        //std::cout << '\n';
    }

    show(m, argv[2]);

    return 0;
}

