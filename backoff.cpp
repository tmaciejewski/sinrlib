#include <iostream>
#include <cstdlib>

#include "backoff.h"

void backoff_algorithm::init(const sinr::model *m)
{
    model = m;
    active.insert(m->get_source());
    // TODO: density
}

bool backoff_algorithm::on_round_end(sinr::uid u, const std::vector<sinr::uid> &messages,
        unsigned round_number)
{
    if (messages.size() > 0)
        active.insert(u);

    if (active.find(u) != active.end())
    {
        if (states[u].counter > 0)
            states[u].counter--;

        if (states[u].counter == 0)
        {
            states[u].phase *= 2;
            if (states[u].phase > 2 * model->get_nodes().size())
            {
                states[u].counter = -1;
            }
            else
            {
                states[u].counter = 1 + rand() % states[u].phase;
            }
            return true;
        }
    }

    return false;
}

bool backoff_algorithm::is_done() 
{
    //std::cout << "active: " << active.size() << std::endl;
    return active.size() == model->get_nodes().size();
}
