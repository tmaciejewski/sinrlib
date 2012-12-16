#include "uniform.h"

#include <cstdlib>
#include <iostream>

namespace sinr {

void uniform_model::generate(unsigned n, double size)
{
    reset();

    while (!choose_component(n))
    {
        double x = size * double(std::rand()) / RAND_MAX;
        double y = size * double(std::rand()) / RAND_MAX;

        add_node(node(x, y));
    }

    node middle_node(size / 2, size / 2);
    uid new_source = 0;
    double best_dist = middle_node - get_nodes()[new_source];

    for (uid u = 0; u < get_nodes().size(); u++)
    {
        double dist = middle_node - get_nodes()[u];
        if (dist < best_dist)
        {
            best_dist = dist;
            new_source = u;
        } 
    }

    set_source(new_source);
}

}
