#!/bin/bash
cd /tmp
wget 'http://ubietylab.net/files/alpha-0.2.4/UbiGraph-alpha-0.2.4-Linux64-Ubuntu-8.04.tgz'
tar xvfz UbiGraph-alpha-0.2.4-Linux64-Ubuntu-8.04.tgz
cd UbiGraph-alpha-0.2.4-Linux64-Ubuntu-8.04
mv include/UbigraphAPI.h /usr/include
mv bin/ubigraph_server /usr/bin
mv examples/C/libubigraphclient.a /usr/lib
cd ..
rm UbiGraph-alpha-0.2.4-Linux64-Ubuntu-8.04
