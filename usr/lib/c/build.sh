#! /bin/sh

echo "building libc"
clang -D_DEFAULT_SOURCE -D_POSIX_SOURCE -Wall -g -I.. -shared -fPIC -o libc.so malloc.c string.c setjmp.c ../vnd/*.c

