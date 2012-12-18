Introduction
============

This is utility library for benchmarking broadcasting algorithms
in SINR wireless network model.

Requirements
============

This library depends on cairo project. Header files are expected
to be in /usr/include/cairo/

Installation
============

Installation is done by autoconf tool:

```
autoreconf --install
./configure
make
make install
```

Running
=======

First you have to generate some networks. For generating a single
network use `generate_model` tool. To view a network use `show_model` tool.
It converts network file into a PDF. Example:

```
generate_model uniform 200 6 uniform.net
show_model uniform.net uniform.pdf

generate_model social 200 6 social.net
show_model social.net social.pdf

generate_model gadget 20 10 gadget.net
show_model gadget.net gadget.pdf
``` 

To run simulation use `sinr` tool. The algorithms that are used in simulation are
hardcoded in `sinr.cpp`. Example:

```
sinr network_dir 10 100,500,100 5,5,1
```

This example runs simulation on 10 networks from network_dir with parameter 1
from 100 to 500 with step 100 and parameter 2 from 5 to 5 with step 1. In network_dir
the directory structure must be like this:

```
network_dir/
   100/
      5/
	1
	2
	.
	.
	.
	10
   200/
      5/
	1
	2
	.
	.
	.
	10
   .
   .
   .
   500/
      5/
	1
	2
	.
	.
	.
	10
```

In general, for t-th trial and p1, p2 being parameters 1 and 2, the tool uses path
network_dir/p1/p2/t to load next network.

The results are presented for each pair of parameters. Example:

```
BackoffWithAck  200     6       95.08   49.5842 13.77   6.90487
AntiBackoff     200     6       87.41   27.3905 13.77   6.34786
DensityKnown    200     6       185.4   44.7569 13.77   13.4641
```

The meaning of columns are: algorithm name, parameter 1, parameter 2, avg. result, standard deviation of result, avg. network diameter, avg. result divided by avg. diameter.


