#include "model.h"

#include <queue>
#include <algorithm>

namespace sinr {

void model::eval(const std::set<uid> & senders, std::map<uid, std::set<uid> > result)
{
    result.clear();

    for (std::set<uid>::iterator sender = senders.begin();
            sender != senders.end(); sender++)
    {
        for (std::map<uid, node>::iterator receiver = nodes.begin();
                receiver != nodes.end(); receiver++)
        {
            bool success = false;

            // node can't send and receive simultanously
            // and can't send to itself

            if (senders.find(receiver->first) == senders.end())
            {
                double interference = 0.0;

                for (std::set<uid>::iterator node = senders.begin();
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
                success_transmit++;
            }
            else
                failed_transmit++;
        }
    }
}

unsigned model::diameter()
{
    unsigned diam_max = 0;
    for (std::map<uid, node>::iterator it = nodes.begin(); it != nodes.end(); it++)
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

unsigned model::diameter_bfs(uid start_uid)
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
        
        queue.pop();

        for (std::set<uid>::iterator u2 = links[u].begin();
                u2 != links[u].end(); u2++)
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
