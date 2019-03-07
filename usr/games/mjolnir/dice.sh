#! /bin/sh

gcc -g -Wall -fno-builtin -DDICE_SRAND_TIME=0 -DTEST=1 -I../../lib -o dice dice.c ../../lib/zero/randmt32.c

