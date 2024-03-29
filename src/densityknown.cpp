#include <iostream>
#include <cstdlib>
#include <cmath>

#include "densityknown.h"

void density_known_algorithm::init(const sinr::model *m)
{
    model = m;
    active.clear();
    states.clear();
    active.insert(m->get_source());
    phase_round = 0;
    gamma = e / (2 * std::sqrt(2));
    eval_ppb();
}

void density_known_algorithm::eval_ppb()
{
    std::map<std::pair<int, int>, unsigned> density;

    for (sinr::uid u = 0; u < model->get_nodes().size(); u++)
    {
        int box_x, box_y;
        box_x = model->get_nodes()[u].x / gamma;
        box_y = model->get_nodes()[u].y / gamma;
        states[u].box_x = box_x;
        states[u].box_y = box_y;
        density[std::make_pair(box_x, box_y)]++;
    }

    for (sinr::uid u = 0; u < model->get_nodes().size(); u++)
    {
        int box_x, box_y;
        box_x = model->get_nodes()[u].x / gamma;
        box_y = model->get_nodes()[u].y / gamma;
        states[u].ppb = C / density[std::make_pair(box_x, box_y)];
        //std::cout << "uid " << u << " has ppb = "
        //    << states[u].ppb << std::endl;
    }
}

bool density_known_algorithm::on_round_end(sinr::uid u, const std::vector<sinr::uid> &messages,
        unsigned round_number)
{
    if (messages.size() > 0)
        active.insert(u);

    if (active.find(u) != active.end())
    {
        int a = phase_round / d;
        int b = phase_round % d;
        if (states[u].box_x % d == a && states[u].box_y % d == b)
        {
            //std::cout << "uid " << u << " got chances: " << a
            //    << " " << b << "\n";
            return static_cast<double>(std::rand()) / RAND_MAX < states[u].ppb;
        }
    }

    return false;
}

bool density_known_algorithm::is_done() 
{
    phase_round++;
    if (phase_round > d * d)
        phase_round = 0;
    //std::cout << "active: " << active.size() << std::endl;
    return active.size() == model->get_nodes().size();
}
