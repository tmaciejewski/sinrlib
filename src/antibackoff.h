#ifndef ANTIBACKOFF_H
#define ANTIBACKOFF_H

#include <map>
#include <set>

#include "sinrlib.h"

class antibackoff_algorithm : public sinr::algorithm
{
    struct state
    {
        unsigned box_x, box_y;
        bool heard;
        state(double x, double y, double gamma)
            : box_x(x / gamma), box_y(y / gamma), heard(false) {}
    };

    std::set<sinr::uid> active;
    const sinr::model *model;
    double gamma, e;
    std::vector<state> states; 

    public:

    antibackoff_algorithm(double e_)
        : e(e_) {}
    void init(const sinr::model *m);
    bool on_round_end(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number);
    bool is_done();
    const char *name() { return "AntiBackoff"; }

    protected:

    bool broadcast(sinr::uid, const std::vector<sinr::uid> &messages);
    bool waiting_for_ack(sinr::uid, const std::vector<sinr::uid> &messages);
    bool send_ack(sinr::uid, const std::vector<sinr::uid> &messages);
};

#endif // BACKOFFACK_H
