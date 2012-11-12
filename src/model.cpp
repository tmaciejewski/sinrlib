#include "model.h"

#include <queue>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdexcept>

namespace sinr {

void model::add_node(node n)
{
    uid u = nodes.size();
    nodes.push_back(n);
    nodes[u].component = u; // initially in its own component

    for (uid other = 0; other < nodes.size() - 1; other++)
    {
        double dist = nodes[u] - nodes[other];
        double power = 1.0 / std::pow(dist, alpha);
        
        nodes[u].power.push_back(power);
        nodes[other].power.push_back(power);
        
        if (dist < range)
        {
            nodes[u].links.push_back(other);
            nodes[other].links.push_back(u);
            component_union(u, other);
        }

        // TODO: change to real range
        if (dist < 1)
        {
            nodes[u].reachables.push_back(other);
            nodes[other].reachables.push_back(u);
        }
    }

    nodes[u].power.push_back(0); // power to itself
}

uid model::component_find(uid u)
{
    uid root = u, tmp;
    std::vector<uid> path;
    while ( (tmp = nodes[root].component) != root )
    {
        path.push_back(root);
        root = tmp;
    }

    // path shortening
    for (std::vector<uid>::iterator it = path.begin();
            it != path.end(); it++)
    {
        nodes[*it].component = root;
    }

    return root;
}

void model::component_union(uid u1, uid u2)
{
    uid root1, root2;

    root1 = component_find(u1);
    root2 = component_find(u2);

    if (root1 != root2)
        nodes[root1].component = root2;
}

bool model::is_connected()
{
    std::set<uid> roots;
    for (uid u = 0; u < nodes.size(); u++)
    {
        uid root = component_find(u);
        roots.insert(root);
    }

    return roots.size() == 1;
}

void model::eval(const std::vector<uid> &senders,
        std::vector< std::vector<uid> > &result) const
{
    result.clear();
    result.resize(nodes.size());

    for (std::vector<uid>::const_iterator sender = senders.begin();
            sender != senders.end(); sender++)
    {
        const std::vector<uid> &reachable = nodes[*sender].reachables;
        for (std::vector<uid>::const_iterator receiver = reachable.begin();
                receiver != reachable.end(); receiver++)
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
                        interference += nodes[*node].power[*receiver];
                    }
                }

                interference += nodes[*receiver].noise;

                if (interference != 0)
                {
                    double sinr = nodes[*sender].power[*receiver] / interference;
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
    for (uid u = 0; u < nodes.size(); u++)
    {
        unsigned diam = diameter_bfs(u);
        if (diam > diam_max)
            diam_max = diam;
    }

    return diam_max;          
}

unsigned model::diameter_bfs(uid start_uid) const
{
    std::vector<bool> visited(nodes.size(), false);
    std::queue<uid> queue;
    std::vector<unsigned> dist(nodes.size(), 0);

    visited[start_uid] = true;
    queue.push(start_uid);
    dist[start_uid] = 0;

    while (queue.size() > 0)
    {
        uid u = queue.front();
        queue.pop();
        for (std::vector<uid>::const_iterator u2 = nodes[u].links.begin();
                u2 != nodes[u].links.end(); u2++)
        {
            if (visited[*u2] == false)
            {
                dist[*u2] = dist[u] + 1;
                visited[*u2] = true;
                queue.push(*u2);
            }
        }
    }

    return *std::max_element(dist.begin(), dist.end());
}    

void model::export_to_pdf(const char *filename) const
{
    int scale = 50;
    double max_x, max_y, min_x, min_y;
    max_x = max_y = min_x = min_y = 0.0;

    for (uid u = 0; u < nodes.size(); u++)
    {
        max_x = std::max(max_x, nodes[u].x);
        max_y = std::max(max_y, nodes[u].y);
        min_x = std::min(min_x, nodes[u].x);
        min_y = std::min(min_y, nodes[u].y);
    }

    int s = std::max(std::abs(max_x - min_x), std::abs(max_y - min_y)) + 1.0;

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

    for (uid u = 0; u < nodes.size(); u++)
    {
        for (std::vector<uid>::const_iterator link_it = nodes[u].links.begin();
                link_it != nodes[u].links.end(); link_it++)
        {
            if (u < *link_it)
            {
                const node &n1 = nodes[u];
                const node &n2 = nodes[*link_it];
                cairo_move_to(cr, scale * n1.x, scale * (s - n1.y));
                cairo_line_to(cr, scale * n2.x, scale * (s - n2.y));
                cairo_stroke(cr);
            }
        }
    }

    for (uid u = 0; u < nodes.size(); u++)
    {
            cairo_arc(cr, scale * nodes[u].x, scale * (s - nodes[u].y), 2, 0, 2*M_PI);
            cairo_set_source_rgb(cr, 1, 0, 0);
            cairo_fill_preserve(cr);
            cairo_set_source_rgb(cr, 0, 0, 0);
            cairo_stroke(cr);
    }
}

void model::save(const char *filename) const
{
    std::ofstream file(filename);
    file.write((char *)&alpha, sizeof(alpha));
    file.write((char *)&beta, sizeof(beta));
    file.write((char *)&range, sizeof(range));
    file.write((char *)&source, sizeof(source));
    for (uid u = 0; u < nodes.size(); u++)
    {
        file.write((char *)&nodes[u].x, sizeof(nodes[u].x));
        file.write((char *)&nodes[u].y, sizeof(nodes[u].y));
    }
}

void model::load(const char *filename)
{
    std::ifstream file(filename);
    
    if (!file)
    {
        std::string msg = "cannot load model: ";
        msg += filename;
        throw std::runtime_error(msg);
    }

    nodes.clear();

    file.read((char *)&alpha, sizeof(alpha));
    file.read((char *)&beta, sizeof(beta));
    file.read((char *)&range, sizeof(range));
    file.read((char *)&source, sizeof(source));
    while (file)
    {
        double x, y;
        file.read((char *)&x, sizeof(x));
        file.read((char *)&y, sizeof(y));
        add_node(node(x, y));
    }
}

bool model::choose_component(unsigned desired_size)
{
    std::vector< std::vector<uid> > all_components(nodes.size());

    if (nodes.size() < desired_size)
        return false;

    for (uid u = 0; u < nodes.size(); u++)
    {
        all_components[component_find(u)].push_back(u);
    }

    for (uid u = 0; u < nodes.size(); u++)
    {
        if (all_components[u].size() >= desired_size)
        {
           extract_nodes(all_components[u]);
           return true;
        }
    }

    return false;
}

void model::extract_nodes(const std::vector<uid> &new_uids)
{
    std::vector<node> old_nodes;
    nodes.swap(old_nodes);
    reset();

    for (unsigned i = 0; i < new_uids.size(); i++)
    {
        uid u = new_uids[i];
        add_node(node(old_nodes[u].x, old_nodes[u].y));
    } 
}

void model::reset()
{
    nodes.clear();
}

} // namespace sinr
