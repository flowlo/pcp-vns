#!/bin/bash

VERSION='?'

if [ ! $(whoami) = 'root' ] ; then
	echo $0 needs root privileges.
	exit
elif [ -f /etc/fedora-release ] ; then
	echo 'Detected Fedora, installing UbiGraph:'
	VERSION='Fedora-9'
elif [ -f /etc/lsb-release ] ; then
	echo 'Detected Ubuntu, installing UbiGraph:'
	VERSION='Ubuntu-8.04'
fi

cd /tmp
wget "http://ubietylab.net/files/alpha-0.2.4/UbiGraph-alpha-0.2.4-Linux64-$VERSION.tgz"
tar xvfz UbiGraph-alpha-0.2.4-Linux64-$VERSION.tgz
cd UbiGraph-alpha-0.2.4-Linux64-$VERSION
mv include/UbigraphAPI.h /usr/include
mv bin/ubigraph_server /usr/bin

if [-f /etc/fedora-release ] ; then
	cd examples/C
	gcc -c client.c -o libubigraphclient.a -lxmlrpc_client
	cd ../..
fi

mv examples/C/libubigraphclient.a /usr/lib
cd ..
rm -rf UbiGraph-alpha-0.2.4-Linux64-$VERSION
