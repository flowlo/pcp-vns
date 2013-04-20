## pcp-vns

[![Build Status](https://travis-ci.org/flowlo/pcp-vns.png?branch=master)](https://travis-ci.org/flowlo/pcp-vns)

A VNS approach to solve the Partition Graph Coloring Problem.

### Approach

We use `onestepCD` from [1] to find an initial solution and then iteratively apply various neighborhood improvement strategies.

## Prerequisites

This implementation uses various components of the [boost C++ libraries](http://www.boost.org/). Needed packages on Ubuntu are [`libboost-dev` and `libboost-program-options-dev`](apt:libboost-dev,libboost-program-options-dev).

To compile with support for [Ubigraph](http://ubietylab.net/ubigraph/) consider installing [`freeglut3` and `libxmlrpc-c3-dev`](apt:freeglut3,libxmlrpc-c3-dev).

### References

[1] G. Li, R. Simha, *"The partition coloring problem and its application to wavelength routing and assignment"*, in: *Proceedings of the First Workshop on Optical Networks*, 2000. (see http://www.seas.gwu.edu/~simha/research/partcolor.ps)

[2] T.F. Noronha, C.C. Ribeiro, *"Routing and wavelength assignment by partition coloring"*, in: *European Journal of Operational Research 171:797-810*, 2006

[3] T.F. Noronha, M.G.C. Resende, C.C. Ribeiro, *"Instances for the routing and wavelength assignment problem"*. (see http://www.ic.uff.br/~celso/grupo/rwa.html)
