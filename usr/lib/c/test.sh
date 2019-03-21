#! /bin/sh

echo "bulding tests"
gcc -O -Wall -g -I.. -o test-string test-string.c string.c
echo "executing tests"
./test

