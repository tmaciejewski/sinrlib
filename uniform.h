#ifndef SINR_UNIFORM_H
#define SINR_UNIFORM_H

#include "model.h"

namespace sinr {

class uniform_model : public model
{
    public:

    uniform_model(config conf, unsigned n, unsigned size,
            double range_mod);
};

} // namespace sinr

#endif // SINR_UNIFORM_H
