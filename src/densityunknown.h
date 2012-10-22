#ifndef SINR_DENSITYUNKNOWN_H
#define SINR_DENSITYUNKNOWN_H

#include <set>

#include "sinrlib.h"

class density_unknown_algorithm : public sinr::algorithm
{
    struct state
    {
        bool has_leader, has_helper;
        sinr::uid leader, helper, selected_leader;
        int box_x, box_y;
        int phase, octant, phase_round[2];
        unsigned ppb;
        bool conflict, candidated, helped;
        std::set<sinr::uid> known_leaders;

        state() {}
        state(double x, double y, double gamma)
            : has_leader(false), has_helper(false),
            box_x(x / gamma), box_y(y / gamma),
            phase(0), octant(0), ppb(1), conflict(false),
            candidated(false), helped(false)
        { phase_round[0] = 0; phase_round[1] = 0; }
    };

    std::set<sinr::uid> active;
    const sinr::model *model;
    std::vector<state> states;
    double e;
    int d, dprim, logn, phase_round;
    double gamma;

    public:

    density_unknown_algorithm(double e_, int C_, int d_, int dprim_)
        : e(e_), d(d_), dprim(dprim_) {}
    void init(const sinr::model *m);
    bool on_round_end(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number);
    bool is_done();
    const char *name() { return "DensityUnknown"; }

    private:

    bool leaders_transmits(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number);
    bool nonleaders_listen(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number);
    bool leader_election(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number);
    bool election_helper(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number);
    void choose_election_helper(sinr::uid u);
    int which_octant(sinr::uid u, sinr::uid v);
};

#endif // SINR_DENSITYUNKNOWN_H
