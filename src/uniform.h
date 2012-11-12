#ifndef SINR_UNIFORM_H
#define SINR_UNIFORM_H

#include "model.h"

namespace sinr {

class uniform_model : public model
{
    public:

    void generate(unsigned n, double size);
};

} // namespace sinr

#endif // SINR_UNIFORM_H
