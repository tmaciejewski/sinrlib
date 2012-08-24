#ifndef SINR_CONFIG_H
#define SINR_CONFIG_H

namespace sinr {

struct config
{
    // transmission power
    double power;

    // path-loss exponent
    double alpha;

    // SINR threshold
    double beta;

    double range;

    config()
    {
        power = 1.0;
        alpha = 2.5;
        beta = 1.0;
        range = 1.0;
    }
};

} // namespace sinr

#endif // SINR_CONFIG_H
