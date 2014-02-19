#!/bin/bash

if [ ! $(whoami) = 'root' ] ; then
	echo $0 needs root privileges.
	exit
elif [ -f /etc/fedora-release ] ; then
	echo 'Detected Fedora, installing dependencies:'
	yum install automake cppunit-devel boost-devel autoconf-archive
elif [ -f /etc/lsb-release ] ; then
	echo 'Detected Ubuntu, installing dependencies:'
	apt-get install libboost-all-dev libboost-dbg automake autoconf-archive libcppunit-dev freeglut3 and libxmlrpc-c3-dev
fi
