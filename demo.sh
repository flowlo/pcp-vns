#!/bin/bash
resolution=$(xdpyinfo | grep dimensions | tr -s ' ' | cut -d ' ' -f 3)
width=$(expr $(echo $resolution | cut -d 'x' -f 1) / 2)
height=$(expr $(echo $resolution | cut -d 'x' -f 2) - 50)

echo 'Starting Ubigraph server'
ubigraph 2> /dev/null &
pid=$!

echo 'Waiting for Ubigraph server to settle'
sleep 0.025

echo 'Establishing layout'
wmctrl -r terminal -e 0,0,0,$width,$height
wmctrl -r ubigraph -e 0,$width,0,$width,$height

echo 'Running'
./pcp -d 4 -n cnd -p /tmp/demo.gv < instances/Table2_random_instances/n20p5t2s5.pcp

echo 'Rendering printed graph'
fdp -Tpdf /tmp/demo.gv > /tmp/demo.pdf 2> /dev/null

echo 'Viewing printed graph'
evince /tmp/demo.pdf 2> /dev/null

echo 'Cleaning temporary files'
rm /tmp/demo.gv /tmp/demo.pdf

echo 'Killing Ubigraph server'
kill $pid 2> /dev/null
