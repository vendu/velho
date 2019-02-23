#! /bin/sh

echo "bulding libc"
clang -Wall -g -I.. -shared -fPIC -o libc.so malloc.c string.c ../vnd/*.c

