#! /bin/sh

echo "bulding tests"
gcc -fno-builtin -Wall -g -I.. -o test test.c string.c
echo "executing tests"
./test

