#! /bin/sh

echo "building libmt"
clang -DVELHO -DMTTHREAD -DMTMTX -DMTFMTX -DMTCOND -Wall -g -I.. -shared -fPIC -o libmt.so *.c

