## pcp-vns

[![Build Status](https://travis-ci.org/lorenzleutgeb/pcp-vns.png?branch=master)](https://travis-ci.org/lorenzleutgeb/pcp-vns)

A VNS approach to solve the Partition Graph Coloring Problem.

### Approach

We use `onestepCD` from [1] to find an initial solution and then iteratively apply various neighborhood improvement strategies.

### Dependencies

This implementation uses various components of the [boost C++ libraries](http://www.boost.org/).

To compile with support for [Ubigraph](http://ubietylab.net/ubigraph/), further libraries are needed.

#### [Ubuntu](apt:libboost-dev,libboost-program-options-dev,freeglut3,freeglut3,libxmlrpc-c3-dev,wmctrl)

| Package                                                                  | Component |
| -------------------------------------------------------------------------|-----------|
| [`libboost-dev`](apt:libboost-dev)                                       | core      |
| [`libboost-program-options-dev`](apt:libboost-program-options-dev)       | core      |
| [`libxmlrpc-c3-dev`](apt:libxmlrpc-c3-dev)                               | ubigraph  |
| [`freeglut3`](apt:freeglut3)                                             | ubigraph  |
| [`wmctrl`](apt:wmctrl)                                                   | scripts   |

#### Fedora

| Package          | Component |
| -----------------|-----------|
| `boost-devel`    | core      |
| `freeglut`       | ubigraph  |
| `xmlrpc-c-devel` | ubigraph  |
| `wmctrl`         | scripts   |

### References

[1] Guangzhi Li, Rahul Simha, [The partition coloring problem and its application to wavelength routing and assignment](http://www.seas.gwu.edu/~simha/research/partcolor.ps), In Proceedings of the First Workshop on Optical Networks, 2000

[2] Thiago F. Noronha, Celso C. Ribeiro, [Routing and wavelength assignment by partition colouring](https://doi.org/10.1016/j.ejor.2004.09.007), In European Journal of Operational Research, Volume 171, Issue 3, 2006, Pages 797-810, ISSN 0377-2217

[3] Thiago F. Noronha, Mauricio G.C. Resende, Celso C. Ribeiro, [Instances for the routing and wavelength assignment problem](http://www.ic.uff.br/~celso/grupo/rwa.html)
