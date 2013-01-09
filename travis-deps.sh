#!/bin/bash

if [ "$TRAVIS" != "true" -o "$CXX" != "clang++" ] ; then
	exit 0
fi

sudo apt-get install -qq -y python-software-properties libboost-all-dev libboost-dbg
sudo apt-get update -qq -y
sudo apt-get upgrade -qq -y
wget -q -O - http://llvm.org/releases/3.2/clang+llvm-3.2-x86_64-linux-ubuntu-12.04.tar.gz | tar -x -z
sudo cp -r -f clang+llvm-3.2-x86_64-linux-ubuntu-12.04/* /
