#!/usr/bin/env python

import glob, sys, os, permute
from os import mkdir
from os.path import abspath, basename
from permute import permute
from subprocess import call

args = '-c -t 30'

if len(sys.argv) < 5:
	print 'Usage: ' + sys.argv[0] + ' <runs> <units> <outdir> <instances>'
	sys.exit(0)

runs = int(sys.argv[1])
variations = permute(sys.argv[2])
output = abspath(sys.argv[3]) + '/'

for i in range(0, runs):
	mkdir(output + '/' + str(i))
	for instance in sys.argv[4:]:
		for variation in variations:
			base = output + '/' + str(i) + '/' + basename(instance) + '.' + variation
			command = './pcp ' + args + ' -p ' + base + '.gv -n ' + variation + ' < ' + instance + ' > ' + base + '.json &'
			print command
			call(command, shell = True)
