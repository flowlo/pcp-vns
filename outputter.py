#!/usr/bin/env python

import json, numpy, sys, os, permute
from sys import argv, exit
from numpy import array, average, std, empty, append
from permute import permute

if len(argv) < 4:
	print 'Usage: ' + argv[0] + ' <units> <input> <instances>'
	exit(0)

units, basedir = argv[1:3]
instances = argv[3:]

for instance in instances:
	infile = os.path.basename(instance)
	bestLeastColors = 10000
	bestInitialColors = 10000
	bestColors = empty(1)
	bestpermut = ''
	bestRunningTime = empty(1)
	for permutation in permute(units):
		leastColors = 10000
		initialColors = 10000
		colors = empty(1)		
		runningTime = empty(1)
		for i in range(1, 30):
			jd = open('output/' + str(i) + '/' + infile + "." + permutation + '.json')
			data = json.load(jd)
			jd.close()
			colors = append(colors, data['colors'])
			initial = data['colors'] + data['improvement']
			runningTime = append(runningTime, sum(time['all']['time']['sum'] for time in data['stats']))

			if data['colors'] < leastColors:
				leastColors = data['colors']
			if initial < initialColors:
				initialColors = initial

		if leastColors < bestLeastColors:
			bestLeastColors = leastColors
			bestInitialColors = initialColors
			bestColors = colors
			bestPermut = permutation
			bestRunningTime = runningTime

	original = open(instance)
	nodes, vertices, partitions = original.readline().split(' ')
	original.close()

	print '\t& '.join(map(str,
	[
		'\\texttt{' + infile + '}',
		nodes,
		vertices,
		partitions[:-1],
		bestInitialColors,
		'\\texttt{' + bestPermut + '}',
		bestLeastColors,
		round(average(bestColors), 2),
		round(std(bestColors), 2),
		round((bestLeastColors - bestInitialColors) / (bestLeastColors / float(-100)), 2),
		round(average(bestRunningTime) / 1000.0, 1)
	]
	)) + '\\\\'
