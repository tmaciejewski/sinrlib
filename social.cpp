#include <cstdlib>
#include <iostream>

#include "social.h"

namespace sinr {

void social_model::uniform_sel(double s, double &x, double &y)
{
    x = s * (static_cast<double>(std::rand()) / RAND_MAX);
    y = s * (static_cast<double>(std::rand()) / RAND_MAX);
}

void social_model::social_sel(int tiles, double e, double &x, double &y)
{
    unsigned i, j, tile;
    double e_x, e_y;

    tile = weight_random();
    i = tile / tiles;
    j = tile % tiles;
    uniform_sel(e, e_x, e_y);
    x = j * e + e_x;
    y = (tiles - i - 1) * e + e_y;
    //std::cout << "e_x: " << e_x << ", e_y: " << e_y
    //    << ", i: " << i << ", j: " << j << '\n'; 
}

void social_model::update_weights(uid u)
{
    int tile = uid_to_tile[u];
    std::set<uid> tiles2;

    for (std::set<uid>::iterator u1 = links[u].begin();
            u1 != links[u].end(); u1++)
    {
        int tile1 = uid_to_tile[*u1];
        std::set<uid> new_for_u1;

        // tell my link about my other links
        new_for_u1.insert(links[u].begin(), links[u].end());
        new_for_u1.erase(*u1);
        sec_links[tile1].insert(new_for_u1.begin(), new_for_u1.end());

        for (std::set<uid>::iterator u2 = links[*u1].begin();
                u2 != links[*u1].end(); u2++)
        {
            if (u != *u2)
            {
                sec_links[uid_to_tile[*u2]].insert(u);
                sec_links[tile].insert(*u2);
            }
        }
    }    
}

unsigned social_model::weight_random()
{
    unsigned sum = 0, level, i;
    double random;

    for (i = 0; i < sec_links.size(); i++)
    {
        sum += sec_links[i].size();
    }

    if (sum == 0)
        return std::rand() % sec_links.size();

    level = sec_links[0].size();
    random = sum * (static_cast<double>(std::rand()) / RAND_MAX);
    for (i = 1; i < sec_links.size() && random > level; i++)
    {
        level += sec_links[i].size();
    }

    return i;
}

void social_model::generate(unsigned n, double size, double e,
        double gamma)
{
    uid current_uid = 0;
    int tiles = std::ceil(size / e);

    reset();
    uid_to_tile.clear();
    sec_links.clear();

    sec_links.resize(tiles * tiles);

    while (!choose_component(n))
    {
        int i, j;
        double x, y;
        double random = static_cast<double>(std::rand()) / RAND_MAX;
        if (random < gamma)
        {
            uniform_sel(size, x, y);
            //std::cout << "uniform node: "
            //    << x << " " << y << '\n';
        }
        else
        {
            social_sel(tiles, e, x, y);
            //std::cout << "social node: "
            //    << x << " " << y << '\n';
        }
        add_node(current_uid, node(x, y));

        i = tiles - 1 - y / e;
        j = x / e;
        uid_to_tile[current_uid] = i * tiles + j;        

        update_weights(current_uid);

        current_uid++;
    }

    set_source(get_nodes().begin()->first);
}

} // namespace sinr
