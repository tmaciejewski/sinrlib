#ifndef SINR_SOCIAL_H
#define SINR_SOCIAL_H

#include <set>
#include <vector>

#include "model.h"

namespace sinr {

class social_model : public model
{
    std::vector<std::set<uid> > sec_links;
    std::vector<int> uid_to_tile;

    public:

    social_model(double a, double b, double r)
        : model(a, b, r) {}
    void generate(unsigned n, double size, double e,
            double gamma);

    protected:

    void uniform_sel(double s, double &x, double &y);
    void social_sel(int tiles, double e, double &x, double &y);
    void update_weights(uid u);
    unsigned weight_random();
};

} // namespace sinr

#endif // SINR_SOCIAL_H
