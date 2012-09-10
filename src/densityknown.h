#ifndef DENSITYKNOWN_H
#define DENSITYKNOWN_H

#include <map>

#include "sinrlib.h"


class density_known_algorithm: public sinr::algorithm
{
    struct state
    {
        int box_x, box_y;
        double ppb;
    };

    std::set<sinr::uid> active;
    const sinr::model *model;
    std::map<sinr::uid, state> states;
    double e, C;
    int d, phase_round;
    double gamma;

    public:

    density_known_algorithm(double e_, int C_, double d_)
        : e(e_), C(C_), d(d_) {}
    void init(const sinr::model *m);
    bool on_round_end(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number);
    bool is_done();
    const char *name() { return "DensityKnown"; }

    protected:

    void eval_ppb();
};

#endif // DENSITYKNOWN_H
