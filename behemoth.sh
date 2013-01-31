#!/bin/bash

host='behemoth.ads.tuwien.ac.at'
target='~/pcp-vns'

if [ ! -f pcp ] ; then
	echo Found no binary to sync with $host
	exit
fi

file pcp | grep -q 'statically linked'

if [ $? -ne 0 ] ; then
	echo Found no statically linked binary to sync with $host
	exit
fi

case $USER in
	lorenz) remote='e1127842' ;;
	moritz) remote='e1127848' ;;
	*) echo Unknown user! ; exit
esac

echo Syncing $remote@$host ...

rsync -aiuz -e ssh pcp $remote@$host:$target
ssh $remote@$host git --git-dir=$target checkout behemoth
