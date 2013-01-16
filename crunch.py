#!/usr/bin/env python

import glob, sys, os, permute
from os import mkdir
from os.path import abspath, basename
from permute import permute
from subprocess import call
from sys import argv, exit

args = '-c -t 30'

if len(sys.argv) < 5:
	print 'Usage: ' + argv[0] + ' <runs> <units> <outdir> <instances>'
	exit(0)

runs = int(argv[1])
permutations = permute(argv[2])
output = abspath(argv[3]) + '/'

for i in range(runs):
	mkdir(output + '/' + str(i))
	for n in argv[4:]:
		for p in permutations:
			base = output + '/' + str(i) + '/' + basename(n) + '.' + p
			call('./pcp ' + args + ' -p ' + base + '.gv -n ' + p + ' < ' + n + ' > ' + base + '.json &', shell = True)
