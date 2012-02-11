#!/bin/sh

if [ "x$ROOTSYS" == "x" ]
then
 echo "\$ROOTSYS is not defined. please define the environment for ROOT."
 exit
else
 sed s#ROOTSYS#$ROOTSYS#g scripts/setup.sh.org > tmp.txt
 mv tmp.txt setup.sh
fi

if [ "x$TARTSYS" == "x" ]
then
 echo "\$TARTSYS is not defined. please define the environment for ANAROOT."
 exit
else
 sed s#TARTSYS#$TARTSYS#g scripts/setup.sh.org > tmp.txt
 mv tmp.txt setup.sh
fi

sed s#PPWWDD#$PWD#g setup.sh > tmp.txt
mv tmp.txt setup.sh

uname=`uname`
if [ "x$uname" == "xDarwin" ]
then
libtool=glibtoolize
else
libtool=libtoolize
fi
srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

#(cd $srcdir; aclocal -I ${OFFLINE_MAIN}/share;\
#libtoolize --force; automake -a --add-missing; autoconf)

(cd $srcdir; aclocal;\
$libtool --force; automake -a --add-missing; autoconf)

$srcdir/configure --enable-maintainer-mode "$@"

