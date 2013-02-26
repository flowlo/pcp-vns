#!/bin/bash
width=$(expr $(xdpyinfo | grep dimensions | tr -s ' ' | cut -d ' ' -f 3 | cut -d 'x' -f 1) / 2)
height=$(expr $(xdpyinfo | grep dimensions | tr -s ' ' | cut -d ' ' -f 3 | cut -d 'x' -f 2) - 40)
killall ubigraph
ubigraph &

echo Waiting for Ubigraph server to settle
sleep 1

wmctrl -r terminal -e 0,0,0,$width,$height
wmctrl -r ubigraph -e 0,$width,0,$width,$height
./pcp -n n -p demo.gv < instances/Table2_random_instances/n20p5t2s5.pcp
fdp -Tpdf demo.gv > demo.pdf
evince demo.pdf &
