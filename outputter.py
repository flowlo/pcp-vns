#!/usr/bin/env python

import json, numpy, pprint, sys, os
from sys import argv
from numpy import array

def permute (word):
	result=[]
	if len(word) == 1:
        	result.append(word)
	else:
		for pos in range(len(word)):
			permutations = permute(word[0:pos]+word[pos+1:len(word)])
			for item in permutations:
				result.append(word[pos] + item)
	return result

instances = argv[1:]


for instance in instances:
	infile = os.path.basename(instance)
	bestLeastColors = 10000
	bestInitialColors = 10000
	bestColors = numpy.empty(1)
	bestpermut = ''
	bestRunningTime = numpy.empty(1)
	for permutation in permute('cdn'):
		leastColors = 10000
		initialColors = 10000
		colors = numpy.empty(1)		
		runningTime = numpy.empty(1)
		for i in range(0, 30):
			json_data = open('output/' + str(i) + '/' + str(i) + infile + "." + permutation + '.json')
			data = json.load(json_data)
			json_data.close()
			colors = numpy.append(colors, data['colors'])
			initial = data['colors'] + data['improvement']
			runningTimeX = 0
			for time in data['stats']:
				runningTimeX = runningTimeX + time['all']['time']['sum']

			runningTime = numpy.append(runningTime, runningTimeX)

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

	avg = numpy.average(bestColors)
	std = numpy.std(bestColors)
	print "\\texttt{" + infile + "} & " + str(bestInitialColors) + " & \\texttt{" + bestPermut + "} & " + str(bestLeastColors) + " & " + str(round(avg, 2)) + " & " + str(round(std, 1)) + " & " + str(round((bestLeastColors - bestInitialColors) / (bestLeastColors / float(-100)), 2)) + " & " + str(round(numpy.average(bestRunningTime) / 1000.0, 1)) + "\\\\" #+ " " + str(round(numpy.std(bestRunningTime) / 1000.0, 1)) 
