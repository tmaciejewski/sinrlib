#ifndef SINR_SIMULATION_H
#define SINR_SIMULATION_H

#include "model.h"

namespace sinr {

class algorithm
{
    public:

    virtual void init(const model *m) = 0;
    virtual bool on_round_end(uid u, const std::vector<uid> & messages, unsigned round_number) = 0;
    virtual bool is_done() = 0;
};

class simulation
{
    const model &m;

    unsigned success;
    unsigned failed;
    unsigned empty_rounds;

    public:

    simulation(const model &m_) : m(m_)
    {
    }

    int run(algorithm &alg, unsigned max_rounds = 1000000);
};

} // namespace sinr

#endif // SINR_SIMULATION_H
