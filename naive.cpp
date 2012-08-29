#include <iostream>
#include <cstdlib>
#include <ctime>

#include "naive.h"

void naive_algorithm::init(const sinr::model *m)
{
    model = m;
    active.insert(m->get_source());
}

bool naive_algorithm::on_round_end(sinr::uid u, const std::vector<sinr::uid> &messages,
        unsigned round_number)
{
    if (messages.size() > 0)
        active.insert(u);

    if (active.find(u) != active.end())
    {
        if (round_number == 0)
            return true;
        else
        {
            double r = (double)(std::rand()) / RAND_MAX;
            return r < (2.0 / model->get_nodes().size());
        }
    }
    return false;
}

bool naive_algorithm::is_done() 
{
    return active.size() == model->get_nodes().size();
}
