#!/usr/bin/env python

vertices, edges, partitions = raw_input().split(' ');

clusters = [];

for i in range(0, int(partitions)):
	clusters.append('');

print 'graph pcp {'

for v in range(0, int(vertices)):
	clusters[int(raw_input())] += '\t\t' + str(v) + ';\n'

i = 0
for cluster in clusters:
	print '\tsubgraph cluster' + str(i) + ' {'
	print cluster
	print '\t}'
	i += 1

for i in range(0, int(edges)):
	print '\t' + ' -- '.join(raw_input().split(' ')) + ';'

print "}"
