## pcp-vns

A VNS approach to solve the Partition Graph Coloring Problem.

### Approach

We use `onestepCD` from [1] to find an initial solution and then iteratively apply various neighborhood improvement strategies.

### References

[1] G. Li, R. Simha, *"The partition coloring problem and its application to wavelength routing and assignment"*, in: *Proceedings of the First Workshop on Optical Networks*, 2000. (see http://www.seas.gwu.edu/~simha/research/partcolor.ps)

[2] T.F. Noronha, C.C. Ribeiro, *"Routing and wavelength assignment by partition coloring", in: *European Journal of Operational Research 171:797-810*, 2006

[3] T.F. Noronha, M.G.C. Resende, C.C. Ribeiro, *"Instances for the routing and wavelength assignment problem"*. (see http://www.ic.uff.br/~celso/grupo/rwa.html)