#!/bin/bash
commit=$(git rev-parse --short HEAD)
job=pcp-vns+$(date +%Y%m%d%H%M)+$commit
base=$HOME/pcp-vns
cd $base
echo qsub -N $job -l 64bit -r y -e $base/jobs/$job.err -o $base/jobs/$job.out \
$base/crunch.py 50 cdn $(find instances/* -type f -name '*.pcp' -or -name '*.col' -or -name '*.in')
