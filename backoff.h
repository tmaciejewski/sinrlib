#include <map>

#include "sinrlib.h"


class backoff_algorithm : public sinr::algorithm
{
    struct state
    {
        int counter, density;
        unsigned phase;
        state() : counter(1), density(1), phase(1) {}
    };

    std::set<sinr::uid> active;
    const sinr::model *model;

    std::map<sinr::uid, state> states; 

    public:

    void init(const sinr::model *m);
    bool on_round_end(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number);
    bool is_done();
};
