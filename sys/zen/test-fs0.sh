#! /bin/sh

echo "building fs0 tests..."
gcc -O -I.. -I../../usr/lib -o chk chk.c
echo -n "executing fs0 tests..."
./chk && echo " SUCCESS"

