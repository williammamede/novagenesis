#!/bin/bash

ME=`whoami`
BASE=`cd ../..; pwd`;

IPCS_S=`ipcs -s | egrep "0x[0-9a-f]+ [0-9]+" | grep $ME | cut -f2 -d" "`
IPCS_M=`ipcs -m | egrep "0x[0-9a-f]+ [0-9]+" | grep $ME | cut -f2 -d" "`
IPCS_Q=`ipcs -q | egrep "0x[0-9a-f]+ [0-9]+" | grep $ME | cut -f2 -d" "`


for id in $IPCS_M; do
  ipcrm -m $id;
done

for id in $IPCS_S; do
  ipcrm -s $id;
done

for id in $IPCS_Q; do
  ipcrm -q $id;
done

rm -rfv /dev/shm/sem.*

# Delete temporary files from IO
rm -rfv $BASE/IO/NRNCS/*.txt
rm -rfv $BASE/IO/NRNCS/*.json
rm -rfv $BASE/IO/NRNCS/*.zip

rm -rfv $BASE/IO/Repository1/*.txt
rm -rfv $BASE/IO/Repository1/*.json
rm -rfv $BASE/IO/Repository1/*.zip

rm -rfv $BASE/IO/Source1/*.txt
rm -rfv $BASE/IO/Source1/*.json
rm -rfv $BASE/IO/Source1/*.zip
