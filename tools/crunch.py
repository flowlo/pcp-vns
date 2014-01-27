#!/usr/bin/env python

import glob, sys, os
from subprocess import call

args = '-c -t 30'

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

if len(sys.argv) < 4:
	print 'Usage: ' + sys.argv[0] + ' <units> <outdir> <instances>'
	sys.exit(0)

variations = permute(sys.argv[1])
output = os.path.abspath(sys.argv[2]) + '/'

for instance in sys.argv[3:]:
	for variation in variations:
		base = output + '/' + os.path.basename(instance) + '.' + variation
		command = './pcp ' + args + ' -p ' + base + '.gv -n ' + variation + ' < ' + instance + ' > ' + base + '.json &'
		print command
		call(command, shell=True)
