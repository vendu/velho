#! /bin/sh

#gcc -g -Wall -O0 -I.. -I../../lib -o cell test.c x11.c cell.c obj.c ../../lib/zero/randmt32.c ../mjolnir/obj.c -lX11
gcc -g -D_REENTRANT=1 -Wall -O0 -I.. -I../../lib -o cell test.c x11.c cell.c ../../lib/zero/randmt32.c -lX11 -pthread

