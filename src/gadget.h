#ifndef SINR_GADGET_H
#define SINR_GADGET_H

#include "model.h"

namespace sinr {

class gadget_model : public model
{
    public:

    void generate(unsigned m, unsigned p, double eps);
};

} // namespace sinr

#endif // SINR_GADGET_H
