#include <iostream>
#include <cstdlib>

#include "backoffack.h"

void backoffack_algorithm::init(const sinr::model *m)
{
    model = m;
    active.clear();
    states.clear();
    active.insert(m->get_source());
    for (sinr::uid u = 0; u < m->get_nodes().size(); u++)
    {
        states.push_back(state());
    }
    states[m->get_source()].broadcasting = true;
    // TODO: density
}
bool backoffack_algorithm::on_round_end(sinr::uid u, const std::vector<sinr::uid> &messages,
        unsigned round_number)
{
    if (states[u].broadcasting && round_number % 1 == 0)
    {
        if (states[u].counter > 0)
        {
            states[u].counter--;
            if (states[u].waiting_for_ack)
            {
                return waiting_for_ack(u, messages);
            }
            else
            {
                return broadcast(u, messages);
            }
        }
        else
        {
            // switched off
            return false;
        }
    }
    else if (!states[u].broadcasting && round_number % 2 == 1)
    {
        return send_ack(u, messages);
    }

    return false;
}

bool backoffack_algorithm::broadcast(sinr::uid u, const std::vector<sinr::uid> &messages)
{
    if (states[u].counter == 0)
    {
        // sending and waiting for ack
        states[u].waiting_for_ack = true;
        states[u].counter = states[u].counter_max + 1;
        return true;
    }

    return false;
}

bool backoffack_algorithm::waiting_for_ack(sinr::uid u, const std::vector<sinr::uid> &messages)
{
    std::vector<sinr::uid> acks;
    for (std::vector<sinr::uid>::const_iterator it = messages.begin();
            it != messages.end(); it++)
    {
        if (states[*it].waken_by == u) // ack for me
        {
            if (states[u].ack_from.find(*it) == states[u].ack_from.end())
                acks.push_back(*it);
        }
    }

    if (acks.size() > 0)
    {
        // got ack so start all over again
        states[u].ack_from.insert(acks.begin(), acks.end());
        states[u].counter = 1;
        states[u].counter_max = 1;
        states[u].waiting_for_ack = false;
    }
    else if (states[u].counter == 0)
    {
        // no ack
        states[u].counter_max *= 2;
        if (states[u].counter_max > 2 * model->get_nodes().size())
        {
            // no more acks
            states[u].counter = -1;
        }
        else
        {
            states[u].waiting_for_ack = false;
            states[u].counter = 1 + std::rand() % states[u].counter_max;;
        }
    }
    return false;
}

bool backoffack_algorithm::send_ack(sinr::uid u, const std::vector<sinr::uid> &messages)
{
    std::vector<sinr::uid> senders;
    for (std::vector<sinr::uid>::const_iterator it = messages.begin();
            it != messages.end(); it++)
    {
        if (states[*it].broadcasting)
            senders.push_back(*it);
    }

    if (senders.size() > 0)
    {
        // wakes up
        sinr::uid sender = senders.front();
        active.insert(u);
        states[u].waken_by = sender;
        states[u].counter_max = states[sender].counter_max;
        states[u].counter = 1 + std::rand() % states[u].counter_max;
    }

    if (active.find(u) != active.end())
    {
        states[u].counter--;
        if (states[u].counter == 0)
        {
            states[u].broadcasting = true;
            states[u].counter_max = 1;
            states[u].counter = 1;
            return true;
        }
    }

    // still sleeping
    return false;
}

bool backoffack_algorithm::is_done() 
{
    //std::cout << "active: " << active.size() << std::endl;
    return active.size() == model->get_nodes().size();
}
