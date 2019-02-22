#! /bin/sh

gcc -g -O -Wall -I../../.. -o stkpool stkpool.c ../../../zero/qrand.c ../../../zero/hash.c -pthread

