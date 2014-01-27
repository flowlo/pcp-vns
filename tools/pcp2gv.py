#!/usr/bin/env python

vertices, edges, partitions = map(int, raw_input().split(' '));

clusters = [];

for i in range(0, partitions):
	clusters.append('');

print 'graph pcp {'
print '\tnode [shape = circle]'
print '\tstyle = rounded'

for v in range(0, vertices):
	clusters[int(raw_input())] += '\t\t' + str(v) + ';\n'

i = 0
for cluster in clusters:
	print '\tsubgraph cluster' + str(i) + ' {'
	print cluster
	print '\t\tlabel = ' + str(i)
	print '\t}'
	i += 1

for i in range(0, edges):
	print '\t' + ' -- '.join(raw_input().split(' ')) + ';'

print '}'
