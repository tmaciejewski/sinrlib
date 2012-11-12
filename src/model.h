#ifndef SINR_MODEL_H
#define SINR_MODEL_H

#include <cmath>
#include <vector>
#include <cairo.h>
#include <cairo-pdf.h>

namespace sinr {

const double default_alpha = 2.5;
const double default_beta  = 1.0;
const double default_epsilon = 0.2;

typedef unsigned uid;

struct node
{
    double x, y;
    double noise;
    std::vector<uid> links, reachables;
    std::vector<double> power;
    uid component;

    node(double x_ = 0.0, double y_ = 0.0, double noise_ = 1)
        : x(x_), y(y_), noise(noise_)
    {
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
    protected:

    double alpha, beta, range;
    std::vector<node> nodes;
    uid source;

    public:

    model() : alpha(default_alpha), beta(default_beta),
              range(1 - default_epsilon)
    {
    }

    model(double a, double b, double r)
        : alpha(a), beta(b), range(r)
    {
    }

    uid get_source() const
    {
        return source;
    }

    const std::vector<node> & get_nodes() const
    {
        return nodes;
    }

    void eval(const std::vector<uid> &senders,
            std::vector< std::vector<uid> > &result) const;
    unsigned diameter() const;
    unsigned diameter_bfs(uid start_uid) const;
    void export_to_pdf(const char *filename) const;
    void save(const char *filename) const;
    void load(const char *filename);

    protected:

    void add_node(node n);
    void component_union(uid u1, uid u2);
    uid  component_find(uid u);
    bool is_connected();
    void plot(cairo_t *cr, int s, int scale) const;
    void set_source(uid u) { source = u; }
    bool choose_component(unsigned desired_size);
    void extract_nodes(const std::vector<uid> &new_uids);
    void reset();
};

} // namespace sinr

#endif // SINR_MODEL_H
