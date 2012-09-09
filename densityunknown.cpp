#include <iostream>
#include <cstdlib>
#include <cmath>
#include <algorithm>

#include "densityunknown.h"

void density_unknown_algorithm::init(const sinr::model *m)
{
    model = m;
    active.clear();
    states.clear();
    active.insert(m->get_source());
    gamma = e / (2 * std::sqrt(2));
    logn = std::log(m->get_nodes().size()) / std::log(2);

    for (sinr::uid u = 0; u < m->get_nodes().size(); u++)
    {
        states[u] = state(m->get_nodes()[u].x, m->get_nodes()[u].y, gamma);
    }

    states[m->get_source()].leader = m->get_source();
    states[m->get_source()].has_leader = true;
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
            choose_election_helper(u);
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
                res = election_helper(u, messages, round_number);
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
    state &st = states[u];
    int a, b;

    a = st.phase_round[0] / d;
    b = st.phase_round[0] % d;

    return (st.box_x % d == a && st.box_y % d == b);
}

bool density_unknown_algorithm::nonleaders_listen(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number)
{
    state &st = states[u];
    if (messages.size() > 0)
    {
        // got any message? wake up
        active.insert(u);
    }

    for (std::vector<sinr::uid>::const_iterator sender_it = messages.begin();
            sender_it != messages.end(); sender_it++)
    {
        // add known leaders
        st.known_leaders.insert(*sender_it);

        // check if this is my leader
        if (states[*sender_it].box_x == st.box_x && states[*sender_it].box_y == st.box_y)
        {
            st.leader = *sender_it;
            st.has_leader = true;
        }
    }

    return false;
}

bool density_unknown_algorithm::leader_election(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number)
{
    state &st = states[u];
    int a, b;
    bool res = false;

    a = st.phase_round[0] / d;
    b = st.phase_round[0] % d;

    if (st.box_x % d == a && st.box_y % d == b)
    {
        if (st.phase_round[2] == 0)
        {
            double p = 1.0 / model->get_nodes().size()
                * std::pow(2, st.phase_round[1]);
            // transmit with ppb
            if (static_cast<double>(std::rand()) / RAND_MAX < p)
            {
                st.candidated = true;
                res = true;
            }
            else
            {
                res = false;
            }
        }
        else if (st.phase_round[2] == 1)
        {
            res = false;
        }
        else if (st.phase_round[2] == 2)
        {
            // heard u? if if transmitted, I'm the leader
            // transmitting if leader
            if (st.candidated)
            {
                if (st.has_helper && std::find(messages.begin(),
                            messages.end(), st.helper) != messages.end())
                {
                    st.leader = u;
                    st.has_leader = true;
                    res = true;
                }
                else
                {
                    st.conflict = true;
                    res = false;
                }
            }
            else
            {
                res = false;
            }
        }
        else if (st.phase_round[2] == 3)
        {
            // other nodes receives the leader's and U's message
            if (st.has_helper && std::find(messages.begin(),
                            messages.end(), st.helper) == messages.end())
            {
                st.conflict = true;
            }
            else
            {
                for (std::vector<sinr::uid>::const_iterator it = messages.begin();
                        it != messages.end(); it++)
                {
                    if (states[*it].box_x == st.box_x && states[*it].box_y == st.box_y)
                    {
                        // discovered a leader
                        st.leader = *it;
                        st.has_leader = true;
                    }
                }
            }
            res = false;
        }
    }
    else
    {
        res = false;
    }

    return res;
}

bool density_unknown_algorithm::election_helper(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number)
{
    state &st = states[u];
    bool res = false;

    if (st.phase_round[2] == 0)
    {
        // wait for leader candidate
        res = false;
    }
    else if (st.phase_round[2] == 1)
    {
        // respond to the leader candidate
        st.helped = false;
        res = false;

        for (std::vector<sinr::uid>::const_iterator it = messages.begin();
                it != messages.end(); it++)
        {
            if (states[*it].has_helper && states[*it].helper == u)
            {
                st.helped = true;
                res = true;
                break;
            }
        }
    }
    else if (st.phase_round[2] == 2)
    {
        // transmit with the leader
        if (st.helped)
        {
            res = true;
        }
        else
        {
            res = false;
        }
    }
    else if (st.phase_round[2] == 3)
    {
        // wait for other
        res = false;
    }

    return res;
}

void density_unknown_algorithm::choose_election_helper(sinr::uid u)
{
    state &st = states[u];
    int helper_dist = -1;
    sinr::uid helper;

    for (std::set<sinr::uid>::iterator leader_it = st.known_leaders.begin();
            leader_it != st.known_leaders.end(); leader_it++)
    {
        int lbox_x, lbox_y;
        double dist;

        lbox_x = states[*leader_it].box_x;
        lbox_y = states[*leader_it].box_y;

        dist = std::sqrt((st.box_x - lbox_x)*(st.box_x - lbox_x)
                + (st.box_y - lbox_y)*(st.box_y - lbox_y));

        if (helper_dist < 0 || dist < helper_dist)
        {
            helper = *leader_it;
            helper_dist = dist;
        }
    }

    if (helper_dist >= 0)
    {
        st.helper = helper;
        st.has_helper = true;
    }
    else
    {
        st.has_helper = false;
    }
}

bool density_unknown_algorithm::is_done() 
{
    //std::cout << "active: " << active.size() << std::endl;
    return active.size() == model->get_nodes().size();
}
