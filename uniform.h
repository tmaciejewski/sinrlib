#ifndef SINR_UNIFORM_H
#define SINR_UNIFORM_H

#include "model.h"

namespace sinr {

class uniform_model : public model
{
    public:

    uniform_model(double a, double b, double r)
        : model(a, b, r) {}
    void generate(unsigned n, unsigned size);
};

} // namespace sinr

#endif // SINR_UNIFORM_H
