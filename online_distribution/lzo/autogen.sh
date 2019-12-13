#!/bin/sh
srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.


$srcdir/configure --enable-shared "$@"


