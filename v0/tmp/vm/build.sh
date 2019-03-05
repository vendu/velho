#! /bin/sh

#CFGLAGS="-DVASDEBUG=1 -g"
#CWARNFLAGS="-Wall -Wundef -Wextra"
#INCLUDES="-I.. -I../.."

clang -O0 -g -Wall -Wundef -Wextra -I../.. -I../../.. -I../../../OS-Zero/usr/lib -o vm *.c ../../vas/*.c

