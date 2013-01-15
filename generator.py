#!/usr/bin/env python

import sys, random
from sys import argv, stderr, exit
from random import randint, sample

if len(argv) >= 4:
	vertices, edges, partitions = map(int, argv[1:4])
	if len(argv) >= 5:
		random.seed(argv[4])
else:
	print 'Usage: ' + argv[0] + ' <vertices> <edges> <partitions> [<seed>]'
	sys.exit(0)

if vertices < 1 or edges < 1 or partitions < 1:
	print >> stderr, 'The number of vertices, edges and partitions must be greater than zero!'
	exit(1)

if vertices < partitions:
	print >> stderr, 'The number of vertices must be greater than or equal to the number of partitions!'
	exit(2)

print ' '.join(argv[1:4])
print '\n'.join(map(str, sample(range(0, partitions), partitions)))

if vertices > partitions:
	print '\n'.join(str(randint(0, partitions - 1)) for i in range(partitions, vertices))

edge = [None, None]
edgeList = [edge]

for i in range(0, edges):
	while edge in edgeList:
		edge = sample(range(0, vertices), 2)
	edgeList.append(edge)
	print ' '.join(map(str, edge))
