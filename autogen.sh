#! /bin/sh

rm -f aclocal.m4

${MAKE:-make} -C aclocal
autoconf; autoheader

rm -rf autom4te.cache/
