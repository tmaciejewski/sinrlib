#include "model.h"

#include <cstdlib>
#include <ctime>

namespace sinr {

class uniform_model : public model
{
    public:

    uniform_model(config conf, unsigned n, unsigned size,
            double range_mod) : model(conf)
    {
        uid current_uid = 0;

        std::srand(std::time(0));

        while (true)
        {
            double x = size * std::rand() / RAND_MAX;
            double y = size * std::rand() / RAND_MAX;

            add_node(current_uid, x, y, range_mod);

            current_uid++;

            if (get_nodes().size() == n)
                return;
        }
    }
};

}
