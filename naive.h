#include "sinrlib.h" 

class naive_algorithm : public sinr::algorithm
{
    std::set<sinr::uid> active;
    const sinr::model *model;

    public:

    void init(const sinr::model *m);
    bool on_round_end(sinr::uid u, const std::vector<sinr::uid> &messages,
            unsigned round_number);
    bool is_done();
};
