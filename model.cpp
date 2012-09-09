#include "model.h"

#include <queue>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>

namespace sinr {

void model::add_node(uid u, const node &n)
{
    if (nodes.find(u) == nodes.end())
    {
        components[u] = u; // initially in its own component
        
        for (std::map<uid, node>::iterator other = nodes.begin();
                other != nodes.end(); other++)
        {
            double dist = n - other->second;
            if (dist < range)
            {
                links[u].push_back(other->first);
                links[other->first].push_back(u);
                component_union(u, other->first);
            }
            
            // TODO: change to real range
            if (dist < 1)
            {
               reachable[other->first].push_back(u);
               reachable[u].push_back(other->first);
            }
        }

        nodes[u] = n;
    }
}

uid model::component_find(uid u)
{
    uid root = u, tmp;
    std::vector<uid> path;
    while ( (tmp = components[root]) != root )
    {
        path.push_back(root);
        root = tmp;
    }

    // path shortening
    for (std::vector<uid>::iterator it = path.begin();
            it != path.end(); it++)
    {
        components[*it] = root;
    }

    return root;
}

void model::component_union(uid u1, uid u2)
{
    uid root1, root2;

    root1 = component_find(u1);
    root2 = component_find(u2);

    if (root1 != root2)
        components[root1] = root2;
}

bool model::is_connected()
{
    std::set<uid> roots;
    for (std::map<uid, node>::iterator it = nodes.begin();
            it != nodes.end(); it++)
    {
        uid root = component_find(it->first);
        roots.insert(root);
    }

    return roots.size() == 1;
}

double model::power(uid sender, uid receiver) const
{
    double dist = nodes.find(sender)->second
        - nodes.find(receiver)->second;
    return 1.0 / std::pow(dist, alpha);
}

void model::eval(const std::vector<uid> &senders,
        std::map<uid, std::vector<uid> > &result) const
{
    result.clear();

    for (std::vector<uid>::const_iterator sender = senders.begin();
            sender != senders.end(); sender++)
    {
        const std::vector<uid> reachables = reachable.find(*sender)->second;
        for (std::vector<uid>::const_iterator receiver = reachables.begin();
                receiver != reachables.end(); receiver++)
        {
            bool success = false;

            // node can't send and receive simultanously
            // and can't send to itself

            if (std::find(senders.begin(), senders.end(), *receiver) == senders.end())
            {
                double interference = 0.0;

                for (std::vector<uid>::const_iterator node = senders.begin();
                        node != senders.end(); node++)
                {
                    if (*node != *sender)
                    {
                        interference += power(*node, *receiver);
                    }
                }

                interference += nodes.find(*receiver)->second.noise;

                if (interference != 0)
                {
                    double sinr = power(*sender, *receiver) / interference;
                    if (sinr > beta)
                        success = true;
                }
                else
                    success = true;
            }

            if (success)
            {
                result[*receiver].push_back(*sender);
            }
        }
    }
}

unsigned model::diameter() const
{
    unsigned diam_max = 0;
    for (std::map<uid, node>::const_iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        unsigned diam = diameter_bfs(it->first);
        if (diam > diam_max)
            diam_max = diam;
    }

    return diam_max;          
}

template <class MapItem>
struct map_val_comp
{
    bool operator()(MapItem it1, MapItem it2)
    {
        return it1.second < it2.second;
    }
};

unsigned model::diameter_bfs(uid start_uid) const
{
    std::set<uid> visited;
    std::queue<uid> queue;
    std::map<uid, unsigned> dist;

    visited.insert(start_uid);
    queue.push(start_uid);
    dist[start_uid] = 0;

    while (queue.size() > 0)
    {
        uid u = queue.front();
        const std::vector<uid> &u_links = links.find(u)->second;      
        queue.pop();

        for (std::vector<uid>::const_iterator u2 = u_links.begin();
                u2 != u_links.end(); u2++)
        {
            if (visited.find(*u2) == visited.end())
            {
                dist[*u2] = dist[u] + 1;
                visited.insert(*u2);
                queue.push(*u2);
            }
        }
    }

    return std::max_element(dist.begin(), dist.end(), map_val_comp<std::map<uid, unsigned>::value_type >())->second;
}    

void model::export_to_pdf(int s, const char *filename) const
{
    int scale = 50;

    cairo_surface_t *surface;
    cairo_t *cr;
    surface = cairo_pdf_surface_create(filename, s * scale, s * scale);
    cr = cairo_create(surface);

    plot(cr, s, scale);

    cairo_show_page(cr);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

void model::plot(cairo_t *cr, int s, int scale) const
{
    cairo_set_line_width(cr, 0.2);
    cairo_set_source_rgb(cr, 0, 0, 1);
    for (std::map<uid, std::vector<uid> >::const_iterator links_it = links.begin();
            links_it != links.end(); links_it++)
    {
        for (std::vector<uid>::const_iterator link_it = links_it->second.begin();
                link_it != links_it->second.end(); link_it++)
        {
            if (links_it->first < *link_it)
            {
                const node &n1 = (nodes.find(links_it->first))->second;
                const node &n2 = (nodes.find(*link_it))->second;
                cairo_move_to(cr, scale * n1.x, scale * (s - n1.y));
                cairo_line_to(cr, scale * n2.x, scale * (s - n2.y));
                cairo_stroke(cr);
            }
        }
    }

    cairo_set_source_rgb(cr, 1, 0, 0);
    for (std::map<uid, node>::const_iterator node_it = nodes.begin();
            node_it != nodes.end(); node_it++)
    {
            cairo_arc(cr, scale * node_it->second.x, scale * (s - node_it->second.y), 2, 0, 2*M_PI);
            cairo_set_source_rgb(cr, 1, 0, 0);
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, 0, 0, 0);
            cairo_stroke(cr);
    }
}

void model::save(const char *filename) const
{
    std::ofstream file(filename);
    file.write(reinterpret_cast<const char *>(&nodes), sizeof(nodes));
    file.write(reinterpret_cast<const char *>(&links), sizeof(links));
    file.write(reinterpret_cast<const char *>(&components), sizeof(components));
    file.write(reinterpret_cast<const char *>(&source), sizeof(source));
}

void model::load(const char *filename)
{
    std::ifstream file(filename);
    file.read(reinterpret_cast<char *>(&nodes), sizeof(nodes));
    file.read(reinterpret_cast<char *>(&links), sizeof(links));
    file.read(reinterpret_cast<char *>(&components), sizeof(components));
    file.read(reinterpret_cast<char *>(&source), sizeof(source));
}

bool model::choose_component(unsigned desired_size)
{
    std::map<uid, std::vector<uid> > all_components;

    if (nodes.size() < desired_size)
        return false;

    for (std::map<uid, node>::iterator it = nodes.begin();
           it != nodes.end(); it++)
    {
        all_components[component_find(it->first)].push_back(it->first);
    }

    for (std::map<uid, std::vector<uid> >::iterator it = all_components.begin();
            it != all_components.end(); it++)
    {
        if (it->second.size() >= desired_size)
        {
           extract_nodes(it->second);
           return true;
        }
    }

    return false;
}

void model::extract_nodes(const std::vector<uid> &new_uids)
{
    std::map<uid, node> old_nodes;
    nodes.swap(old_nodes);
    links.clear();
    components.clear();

    for (std::vector<uid>::const_iterator it = new_uids.begin();
           it != new_uids.end(); it++)
    {
       add_node(*it, old_nodes[*it]);
    } 
}

void model::reset()
{
    nodes.clear();
    links.clear();
    components.clear();
    reachable.clear();
}

} // namespace sinr
