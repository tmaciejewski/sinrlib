#include <iostream>
#include <cstdlib>
#include <cmath>

#include "antibackoff.h"

void antibackoff_algorithm::init(const sinr::model *m)
{
    model = m;
    active.clear();
    states.clear();
    gamma = e / (6 * std::sqrt(2));
    for (sinr::uid u = 0; u < m->get_nodes().size(); u++)
    {
        states.push_back(state(m->get_nodes()[u].x,
                    m->get_nodes()[u].y, gamma));
    }
    active.insert(m->get_source());
}

bool antibackoff_algorithm::on_round_end(sinr::uid u, const std::vector<sinr::uid> &messages,
        unsigned round_number)
{
    if (messages.size() > 0)
    {
        active.insert(u);
    }

    if (active.find(u) != active.end() && states[u].heard == false)
    {
        for (unsigned i = 0; i < messages.size(); i++)
        {
            if (states[u].box_x == states[messages[i]].box_x &&
                    states[u].box_y == states[messages[i]].box_y)
            {
                //std::cout << u << " heard " << messages[i] << ", switches off\n";
                states[u].heard = true;
                return false;
            }
        }

        double k = std::log(model->get_nodes().size()) / std::log(2);
        double ppb = 1.0 / std::pow(2, 1 + round_number % int(std::ceil(k)));
        return static_cast<float>(std::rand()) / RAND_MAX < ppb;
    }
    else
    {
        return false;
    }
}

bool antibackoff_algorithm::is_done() 
{
    //std::cout << "active: " << active.size() << std::endl;
    return active.size() == model->get_nodes().size();
}
