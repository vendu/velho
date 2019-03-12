#! /bin/sh

echo "bulding tests"
gcc -O -Wall -g -I.. -o test test.c string.c
echo "executing tests"
./test

