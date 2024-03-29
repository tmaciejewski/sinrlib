#include "gadget.h"

#include <cstdlib>
#include <cmath>
#include <iostream>

namespace sinr {

void gadget_model::generate(unsigned m, unsigned p, double eps)
{
    uid current_uid = 0;
    double s = range / std::sqrt(2);
    double source_x = 1;
    double source_y = 1;

    reset();

    add_node(node(source_x, source_y));
    current_uid++;

    while (m-- > 0)
    {
        std::vector<uid> recent_nodes;
        for (unsigned i = 1; i < p; i++)
        {
            double x, y, mod_y;
            mod_y = -eps + eps * 2 * (static_cast<double>(std::rand()) / RAND_MAX);
            x = source_x + s * (static_cast<double>(std::rand()) / RAND_MAX);
            y = source_y + s + mod_y;

            add_node(node(x, y));
            recent_nodes.push_back(current_uid);
            current_uid++;
        }

        uid next_source = recent_nodes[std::rand() % recent_nodes.size()];
        source_x = nodes[next_source].x;
        source_y = nodes[next_source].y + s;

        add_node(node(source_x, source_y));
        current_uid++;
    }

    set_source(0);
}

}
