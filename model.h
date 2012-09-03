#ifndef SINR_MODEL_H
#define SINR_MODEL_H

#include <cmath>
#include <map>
#include <set>
#include <vector>
#include <cairo.h>
#include <cairo-pdf.h>

namespace sinr {

typedef unsigned uid;

struct node
{
    double x, y;
    double noise;

    node(double x_ = 0.0, double y_ = 0.0, double noise_ = 1)
        : x(x_), y(y_), noise(noise_)
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
    protected:

    double alpha, beta, range;
    std::map<uid, node> nodes;
    std::map<uid, std::set<uid> > links;
    std::map<uid, std::set<uid> > reachable;
    std::map<uid, uid> components;
    uid source;

    public:

    model(double a, double b, double r)
        : alpha(a), beta(b), range(r)
    {
    }

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

    double power(uid sender, uid receiver) const;
    void eval(const std::vector<uid> &senders,
            std::map<uid, std::vector<uid> > &result) const;
    unsigned diameter() const;
    unsigned diameter_bfs(uid start_uid) const;
    void export_to_pdf(int s, const char *filename) const;
    void save(const char *filename) const;
    void load(const char *filename);

    protected:

    void add_node(uid u, const node &n);
    void component_union(uid u1, uid u2);
    uid  component_find(uid u);
    bool is_connected();
    void plot(cairo_t *cr, int s, int scale) const;
    void set_source(uid u) { source = u; }
    bool choose_component(unsigned desired_size);
    void extract_nodes(const std::set<uid> &new_uids);
    void reset();
};

} // namespace sinr

#endif // SINR_MODEL_H
