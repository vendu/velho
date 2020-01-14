#! /bin/sh

echo "generating bit-global.c"
gcc -O -o bit-gen bit-gen.c
./bit-gen > bit-global.c

