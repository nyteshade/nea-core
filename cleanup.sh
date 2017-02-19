#!/bin/sh

find . -type f \(\
     -iname \*.o -o \
     -iname \*.uaem -o \
     -iname \*.info -o \
     -iname \*.lnk\
     \) -exec rm -v {} \;
