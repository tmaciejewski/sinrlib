#include "model.h"

#include <queue>
#include <algorithm>

namespace sinr {


void model::add_node(uid u, double x, double y, double range_mod)
{
    if (nodes.find(u) == nodes.end())
    {
        node n = node(x, y);
        for (std::map<uid, node>::iterator other = nodes.begin();
                other != nodes.end(); other++)
        {
            if (n - other->second < range_mod)
            {
                links[u].insert(other->first);
                links[other->first].insert(u);
            }
        }
        nodes[u] = n;
    }
}


void model::eval(const std::set<uid> & senders,
        std::map<uid, std::set<uid> > result) const
{
    result.clear();

    for (std::set<uid>::iterator sender = senders.begin();
            sender != senders.end(); sender++)
    {
        for (std::map<uid, node>::const_iterator receiver = nodes.begin();
                receiver != nodes.end(); receiver++)
        {
            bool success = false;

            // node can't send and receive simultanously
            // and can't send to itself

            if (senders.find(receiver->first) == senders.end())
            {
                double interference = 0.0;

                for (std::set<uid>::const_iterator node = senders.begin();
                        node != senders.end(); node++)
                {
                    if (*node != *sender)
                    {
                        interference += power(*node, receiver->first);
                    }
                }

                interference += receiver->second.noise;

                if (interference != 0)
                {
                    double sinr = power(*sender, receiver->first) / interference;
                    if (sinr > conf.beta)
                        success = true;
                }
                else
                    success = true;
            }

            if (success)
            {
                result[receiver->first].insert(*sender);
            }
        }
    }
}

unsigned model::diameter() const
{
    unsigned diam_max = 0;
    for (std::map<uid, node>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        unsigned diam = diameter_bfs(it->first);
        if (diam > diam_max)
            diam_max = diam;
    }

    return diam_max;          
}

template <class MapItem>
struct map_val_comp
{
    bool operator()(MapItem it1, MapItem it2)
    {
        return it1.second < it2.second;
    }
};

unsigned model::diameter_bfs(uid start_uid) const
{
    std::set<uid> visited;
    std::queue<uid> queue;
    std::map<uid, unsigned> dist;

    visited.insert(start_uid);
    queue.push(start_uid);
    dist[start_uid] = 0;

    while (queue.size() > 0)
    {
        uid u = queue.front();
        const std::set<uid> &u_links = links.find(u)->second;      
        queue.pop();

        for (std::set<uid>::const_iterator u2 = u_links.begin();
                u2 != u_links.end(); u2++)
        {
            if (visited.find(*u2) == visited.end())
            {
                dist[*u2] = dist[u] + 1;
                visited.insert(*u2);
                queue.push(*u2);
            }
        }
    }

    return std::max_element(dist.begin(), dist.end(), map_val_comp<std::map<uid, unsigned>::value_type >())->second;
}    

} // namespace sinr
