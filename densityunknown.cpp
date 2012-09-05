#include <iostream>
#include <cstdlib>
#include <cmath>

#include "densityunknown.h"

void density_unknown_algorithm::init(const sinr::model *m)
{
    model = m;
    active.clear();
    states.clear();
    active.insert(m->get_source());
    gamma = e / (2 * std::sqrt(2));
    logn = std::log(m->get_nodes().size()) / std::log(2);

    for (std::map<sinr::uid, sinr::node>::const_iterator it = m->get_nodes().begin();
            it != m->get_nodes().end(); it++)
    {
        states[it->first] = state(it->second.x, it->second.y, gamma);
    }

    states[m->get_source()].leader = m->get_source();
}

bool density_unknown_algorithm::on_round_end(sinr::uid u,
        const std::vector<sinr::uid> &messages,
        unsigned round_number)
{
    state &st = states[u];
    bool res;

    if (st.phase == 0)
    {
        // leader transmission phase
        if (st.has_leader && u == st.leader)
            res = leaders_transmits(u, messages, round_number);
        else
            res = nonleaders_listen(u, messages, round_number);
        st.phase_round[0]++;
        if (st.phase_round[0] > d*d)
            st.phase = 1;
    }
    else if (st.phase == 1)
    {
        // if not a leader
        if (st.has_leader && u != st.leader)
            res = nonleaders_listen(u, messages, round_number);
        else
            res = false;

        st.phase = 2;
        st.phase_round[0] = 0;
        st.phase_round[1] = 0;
        st.phase_round[2] = 0;
        st.conflict = false;
        
        if (!st.has_leader)
        {
            // no leader in a box
            st.helper = election_helper(u);
            st.has_helper = true;
        }
    }
    else
    {
        if (active.find(u) != active.end())
        {
            // leader election phase
            if (!st.conflict && !st.has_leader && st.has_helper)
            {
                // don't have leader and no conflict so far
                res = leader_election(u, messages, round_number);
            }
            else if (st.has_leader && st.leader == u)
            {
                // u is leader and maybe also a helper
                res = election_help(u, messages, round_number);
            }
            else
            {
                // not a leader, maybe conflict, maybe slave
                res = false;
            }
        }
        else
        {
            res = false;
        }

        st.phase_round[2]++;

        if (st.phase_round[2] > 2)
        {
            st.phase_round[2] = 0;
            st.phase_round[1]++;
        }

        if (st.phase_round[1] > logn)
        {
            st.phase_round[1] = 0;
            st.phase_round[0]++;
        }

        if (st.phase_round[0] >= dprim*dprim)
        {
            st.phase = 0;
            st.phase_round[0] = 0;
        }
    }

    return res;
}

bool density_unknown_algorithm::leaders_transmits(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number)
{
    return false;
}

bool density_unknown_algorithm::nonleaders_listen(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number)
{
    return false;
}

bool density_unknown_algorithm::leader_election(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number)
{
    return false;
}

bool density_unknown_algorithm::election_help(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number)
{
    return false;
}

sinr::uid density_unknown_algorithm::election_helper(sinr::uid u)
{
    return u;
}

bool density_unknown_algorithm::is_done() 
{
    //std::cout << "active: " << active.size() << std::endl;
    return active.size() == model->get_nodes().size();
}
