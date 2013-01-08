#!/usr/bin/env python

import sys, random
from sys import argv
from random import randint, sample

if len(argv) >= 4:
	vertices, edges, partitions = map(int, argv[1:4])
	if len(argv) >= 5:
		random.seed(argv[4])
else:
	print 'Usage: ' + argv[0] + ' <vertices> <edges> <partitions> [<seed>]'
	sys.exit(0)

print ' '.join(argv[1:4])
print '\n'.join(str(partition) for partition in sample(range(0, partitions), partitions))
print '\n'.join(str(randint(0, partitions)) for i in range(partitions, vertices))

edge = [None, None]
edgeList = [edge]

for i in range(0, edges):
	while edge in edgeList:
		edge = sample(range(0, vertices), 2)
	edgeList.append(edge)
	print ' '.join(str(vertex) for vertex in edge)
