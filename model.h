#ifndef SINR_MODEL_H
#define SINR_MODEL_H

#include "config.h"

#include <cmath>
#include <map>
#include <set>
#include <cairo.h>
#include <cairo-pdf.h>

namespace sinr {

typedef unsigned uid;

struct node
{
    double x, y;
    double noise;

    node(double x_ = 0.0, double y_ = 0.0) : x(x_), y(y_)
    {
    }

    bool operator==(const node & other) const
    {
        return x == other.x && y == other.y;
    }
        
    double operator-(const node & other) const
    {
        double dx = other.x - x;
        double dy = other.y - y;
        return std::sqrt(dx*dx + dy*dy);
    }
};    

class model
{
    config conf;
    std::map<uid, node> nodes;
    std::map<uid, std::set<uid> > links;
    std::map<uid, uid> components;
    uid source;

    // TODO
    // reachable = {}
    // power_cache = {}

    public:

    model(const config &c) : conf(c)
    {
    }

    double power(uid sender, uid receiver) const
    {
        // TODO
        //try:
        //    return self.power_cache[(sender, receiver)]
        //except KeyError:
        double dist = nodes.find(sender)->second
            - nodes.find(receiver)->second;
        double p = conf.power / std::pow(dist, conf.alpha);
        // self.power_cache[(sender, receiver)] = p
        return p;
    }

    void eval(const std::set<uid> & senders,
            std::map<uid, std::set<uid> > result) const;

    unsigned diameter() const;

    unsigned diameter_bfs(uid start_uid) const;

    uid get_source() const
    {
        return source;
    }

    const std::map<uid, node> & get_nodes() const
    {
        return nodes;
    }

    const std::map<uid, std::set<uid> > & get_links() const
    {
        return links;
    }

    void export_to_pdf(int s, const char *filename) const;

    void save(const char *filename) const;

    void load(const char *filename);

    protected:

    void add_node(uid u, double x, double y, double range_mod);
    void component_union(uid u1, uid u2);
    uid  component_find(uid u);
    bool is_connected();
    void plot(cairo_t *cr, int s, int scale) const;
};

} // namespace sinr

#endif // SINR_MODEL_H
