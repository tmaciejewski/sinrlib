#include "simulation.h"

#include <iostream>

namespace sinr {

int simulation::run(algorithm &alg, unsigned max_rounds)
{
    unsigned round_number = 0;
    unsigned warn_step = max_rounds / 10;
    std::set<uid> senders;

    success = 0;
    failed = 0;
    empty_rounds = 0;

    alg.init(m);

    while (!alg.is_done())
    {
        std::set<uid> receivers;
        std::map<uid, std::set<uid> > messages;

        if (round_number % warn_step == 0 && round_number > 0)
            std::cout << "round:" << round_number << std::endl;

        if (round_number > max_rounds)
            return -1;

        if (senders.size() == 0)
            empty_rounds++;

        m.eval(senders, messages);
        senders.clear();
        for (std::map<uid, node>::const_iterator u = m.get_nodes().begin();
                u != m.get_nodes().end(); u++)
        {
            if (alg.on_round_end(u->first, messages[u->first], round_number))
            {
                senders.insert(u->first);
            }
        }

        round_number++;
    }
    return round_number;
}

}