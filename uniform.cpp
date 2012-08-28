#include "uniform.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

namespace sinr {

void uniform_model::generate(unsigned n, unsigned size)
{
    uid current_uid = 0;

    std::srand(std::time(0));

    while (!choose_component(n))
    {
        double x = size * double(std::rand()) / RAND_MAX;
        double y = size * double(std::rand()) / RAND_MAX;

        add_node(current_uid, node(x, y));

        current_uid++;
    }

    set_source(get_nodes().begin()->first);
}

}
