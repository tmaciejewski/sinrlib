#ifndef SINR_MODEL_H
#define SINR_MODEL_H

#include "config.h"

#include <cmath>
#include <map>
#include <set>

namespace sinr {

typedef unsigned uid;

struct node
{
    double x, y;
    double noise;

    node(double x_ = 0.0, double y_ = 0.0) : x(x_), y(y_)
    {
    }

    bool operator==(const node & other)
    {
        return x == other.x && y == other.y;
    }
        
    double operator-(const node & other)
    {
        double dx = other.x - x;
        double dy = other.y - y;
        return std::sqrt(dx*dx + dy*dy);
    }

    //TODO
    //def __str__(self):
    //    return 'Node(%s, %s)' % (self.x, self.y)
};    

class model
{
    config conf;
    std::map<uid, node> nodes;
    std::map<uid, std::set<uid> > links;
    unsigned failed_transmit, success_transmit;
    // TODO
    // reachable = {}
    // power_cache = {}

    public:

    model(const config &c) : conf(c), failed_transmit(0), success_transmit(0)
    {
    }

    double power(uid sender, uid receiver)
    {
        // TODO
        //try:
        //    return self.power_cache[(sender, receiver)]
        //except KeyError:
        double dist = nodes[sender] - nodes[receiver];
        double p = conf.power / std::pow(dist, conf.alpha);
        // self.power_cache[(sender, receiver)] = p
        return p;
    }

    void eval(const std::set<uid> & senders, std::map<uid, std::set<uid> > result);
    // TODO: connected components

    unsigned diameter();

    unsigned diameter_bfs(uid start_uid);

// TODO
//    def show(self, active = set(), title = 'Network'):
//        self.plot(active, title)
//        matplotlib.pyplot.show()
//
//    def plot(self, active, title):
//        matplotlib.pyplot.cla()
//        for s in self.links:
//            for r in self.links[s]:
//                if s < r:
//                    xs = [self.nodes[s].x, self.nodes[r].x]
//                    ys = [self.nodes[s].y, self.nodes[r].y]
//                    matplotlib.pyplot.plot(xs, ys, 'b')
//
//        xs1 = [self.nodes[uid].x for uid in self.nodes if uid not in active]
//        ys1 = [self.nodes[uid].y for uid in self.nodes if uid not in active]
//        xs2 = [self.nodes[uid].x for uid in active]
//        ys2 = [self.nodes[uid].y for uid in active]
//        matplotlib.pyplot.title(title)
//        matplotlib.pyplot.plot(xs1, ys1, 'or', markersize = 8)
//        matplotlib.pyplot.plot(xs2, ys2, 'oy', markersize = 8)
//
//    def export(self, filename, active, title):
//        self.plot(active, title)
//        matplotlib.pyplot.savefig(filename)
//
//    def save(self, filename):
//        f = open(filename, 'wb')
//        pickle.dump(self.nodes, f)
//        pickle.dump(self.links, f)
//        f.close()
//
//    def load(self, filename):
//        f = open(filename, 'rb')
//        self.nodes = pickle.load(f)
//        self.links = pickle.load(f)
//
};

} // namespace sinr

#endif // SINR_MODEL_H
