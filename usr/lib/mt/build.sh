#! /bin/sh

echo "building libmt"
clang -DMTMTX -DMTFMTX -DMTCOND -Wall -g -I.. -shared -fPIC -o libmt.so *.c

