#!/usr/bin/env python

import json, numpy, sys, os, permute
from sys import argv, exit
from numpy import array, std, empty, append, mean

def permute (word):
	result = []
	if len(word) == 1:
		result.append(word)
	else:
		for pos in range(len(word)):
			permutations = permute(word[0:pos] + word[pos + 1:len(word)])
			for item in permutations:
				result.append(word[pos] + item)
	return result

if len(argv) < 4:
	print 'Usage: ' + argv[0] + ' <units> <runs> <instances>'
	exit(0)

units = argv[1]
runs = int(argv[2])
instances = argv[3:]

for instance in instances:
	infile = os.path.basename(instance)
	bestLeastColors = 10000
	bestInitialColors = 10000
	bestColors = empty(0)
	bestpermut = ''
	bestRunningTime = empty(0)
	for permutation in permute(units):
		leastColors = 10000
		initialColors = 10000
		colors = empty(0)		
		runningTime = empty(0)
		for i in range(0, runs):
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
			initialColors,
			'\\texttt{' + permutation + '}',
			leastColors,
			round(mean(colors), 2),
			round(std(colors), 2),
			round((leastColors - initialColors) / (leastColors / float(-100)), 2),
			round(mean(runningTime) / 1000.0, 1)
		]
		)) + '\\\\'
