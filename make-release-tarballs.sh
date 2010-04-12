#!/bin/sh

VERSION="INSERT-RELEASE-TAG-HERE"
exit

svn export https://svn-i2.informatik.rwth-aachen.de/repos/libalf/tags/${VERSION} release

cd release

rm -Rf libalf/testsuites/sample-automata

tar jcf libalf-${VERSION}.tar.bz2		libalf
tar jcf "libAMoRE(++)-${VERSION}.tar.bz2"	libAMoRE libAMoRE++ 
tar jcf libmVCA-${VERSION}.tar.bz2		libmVCA
tar jcf liblangen-${VERSION}.tar.bz2		liblangen 
tar jcf finite-automata-tool-${VERSION}.tar.bz2	finite-automata-tool 

mv *.tar.bz2 ../

cd ..

rm -Rf release

