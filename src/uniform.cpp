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

    set_source(std::rand() % get_nodes().size());
}

}
