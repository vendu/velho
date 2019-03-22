#! /bin/sh

echo "bulding test-string"
gcc -O -Wall -g -I.. -o test-string test-string.c string.c
echo "executing test-string"
./test-string

