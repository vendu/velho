#! /bin/sh

echo "building libc"
clang -Wall -g -I.. -shared -fPIC -o libc.so malloc.c string.c ../vnd/*.c

