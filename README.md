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

autoreconf --install
./configure
make
make install

Running
=======

```
show_model uniform uniform.pdf
show_model social social.pdf
show_model gadget gadget.pdf
sinr 10 100,500,100 5,5,1
```
