#!/usr/bin/env python

vertices, edges, partitions = map(int, raw_input().split(' '));

clusters = [];

for i in range(partitions):
	clusters.append('')

for v in range(vertices):
	clusters[int(raw_input())] += '\t\t' + str(v) + ';\n'

print '\n'.join(
	['graph pcp {', '\tnode [shape = circle]', '\tstyle = rounded'] +
	['\t' + ' -- '.join(raw_input().split(' ')) + ';' for i in range(edges)] +
	[item for i, cluster in enumerate(clusters) for item in (['\tsubgraph cluster' + str(i) + ' {', cluster, '\t\tlabel = ' + str(i), '\t}'])]
)
print '}'
