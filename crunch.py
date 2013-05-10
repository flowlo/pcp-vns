#!/usr/bin/env python

import glob, sys, os, permute
from os import mkdir
from os.path import abspath, basename
from subprocess import call
from sys import argv, exit

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

args = '-s 10 -i 20 -u 1000 -t 600'

if len(sys.argv) < 5:
	print 'Usage: ' + argv[0] + ' <runs> <units> <outdir> <instances>'
	exit(0)

runs = int(argv[1])
permutations = permute(argv[2])
output = abspath(argv[3]) + '/'

for i in range(runs):
	mkdir(output + str(i))
	for n in argv[4:]:
		for p in permutations:
			base = output + '/' + str(i) + '/' + basename(n) + '.' + p
			call('./pcp ' + args + ' -n ' + p + ' < ' + n + ' > ' + base + '.json &', shell = True)
