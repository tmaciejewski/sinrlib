#include <map>
#include <set>

#include "sinrlib.h"

class backoffack_algorithm : public sinr::algorithm
{
    struct state
    {
        bool broadcasting, waiting_for_ack;
        int counter;
        unsigned counter_max, density;
        std::set<sinr::uid> ack_from;
        state() : broadcasting(false), waiting_for_ack(false),
        counter(1), counter_max(1), density(1) {}
    };

    std::set<sinr::uid> active;
    const sinr::model *model;

    std::map<sinr::uid, state> states; 

    public:

    void init(const sinr::model *m);
    bool on_round_end(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number);
    bool is_done();

    protected:

    bool broadcast(sinr::uid, const std::vector<sinr::uid> &messages);
    bool waiting_for_ack(sinr::uid, const std::vector<sinr::uid> &messages);
    bool send_ack(sinr::uid, const std::vector<sinr::uid> &messages);
};
