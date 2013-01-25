#!/bin/bash
if [ ! -f pcp ] ; then
	echo Found no binary to push to behemoth.ads.tuwien.ac.at
	exit
fi

ldd pcp | grep 'not a dynamic executable' > /dev/null

if [ $? -ne 0 ] ; then
	echo Found no statically linked binary to push to behemoth.ads.tuwien.ac.at
	exit
fi

echo Pushing to behemoth.ads.tuwien.ac.at ...

case $USER in
	lorenz) remote='e1127842' ;;
	moritz) remote='e1127848'
esac

rsync -aiuz -e ssh pcp $remote@behemoth.ads.tuwien.ac.at:~/pcp
