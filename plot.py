#!/usr/bin/python

import matplotlib.pyplot, sys

def plot(filename, args, style, label):
    f = open(filename, 'r')

    xs = []
    ys = []

    x, y = [int(i) for i in args.split(',')]

    for line in f:
        vals = line.split()
        xs.append(vals[x - 1])
        ys.append(vals[y - 1])

    matplotlib.pyplot.plot(xs, ys, style, linewidth=2, label=label)

#matplotlib.pyplot.title(title)
#matplotlib.pyplot.xlim([0, 4000])
plot(sys.argv[1], sys.argv[4], '-', 'RandBroadcast')
plot(sys.argv[2], sys.argv[4], '--', 'Backoff')
plot(sys.argv[3], sys.argv[4], ':', 'AntiBackoff')
matplotlib.pyplot.legend(loc=0)
matplotlib.pyplot.xlabel('n')
matplotlib.pyplot.ylabel(sys.argv[5])

if len(sys.argv) < 7:
    matplotlib.pyplot.show()
else:
    matplotlib.pyplot.savefig(sys.argv[6])
