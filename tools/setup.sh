#!/bin/bash

if [ ! $(whoami) = 'root' ] ; then
	echo $0 needs root privileges. It will install dependency packages and UbiGraph.
	exit
fi

if [ -f /etc/fedora-release ] ; then
	echo 'Detected Fedora, installing dependencies:'
	yum install automake cppunit-devel boost-devel autoconf-archive
elif [ -f /etc/lsb-release ] ; then
	if [ $TRAVIS ] ; then
		apt-get install --force-yes libboost-all-dev libboost-dbg automake autoconf-archive libcppunit-dev
	else
		echo 'Detected Ubuntu, installing dependencies:'
		apt-get install libboost-all-dev libboost-dbg automake autoconf-archive libcppunit-dev
		if [ $(pidof X) ] ; then
			cd /tmp
			wget 'http://ubietylab.net/files/alpha-0.2.4/UbiGraph-alpha-0.2.4-Linux64-Ubuntu-8.04.tgz'
			tar xvfz UbiGraph-alpha-0.2.4-Linux64-Ubuntu-8.04.tgz
			cd UbiGraph-alpha-0.2.4-Linux64-Ubuntu-8.04
			mv include/UbigraphAPI.h /usr/include
			mv bin/ubigraph_server /usr/bin
			mv examples/C/libubigraphclient.a /usr/lib
			cd ..
			rm -rf UbiGraph-alpha-0.2.4-Linux64-Ubuntu-8.04
		fi
	fi
fi
