#! /bin/sh

echo "building test"
gcc -O -I../../../../../lib -g -O -o test v0.c ham.c clz.c ok.c

