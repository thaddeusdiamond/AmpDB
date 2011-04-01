#!/bin/bash

DB=utils/dummy-db
DBNAME=`basename $DB`

for CL in 1500 2000 2500 3000 4000; do
  for WAIT in 1000 3000 5000 7500 10000; do
    perl -lpe '$_="wait_time='$WAIT'" if /^wait_time/' test.conf > t;
    mv -f t test.conf;
    utils/cluster test.conf $DB preprocessor/preproc mediator/mediator -q &

    sleep 5
    echo "CL=$CL WAIT=$WAIT"
    echo "CL=$CL WAIT=$WAIT" >> confs.txt
    coproc utils/client -m 0 test.conf -b $CL >> confs.txt
    CLIENT1_PID=$COPROC_PID
    coproc utils/client -m 1 test.conf -b $CL >> confs.txt
    CLIENT2_PID=$COPROC_PID
    coproc utils/client -m 2 test.conf -b $CL >> confs.txt
    CLIENT3_PID=$COPROC_PID
    sleep 2
    kill -INT $CLIENT1_PID
    kill -INT $CLIENT2_PID
    kill -INT $CLIENT3_PID
    sleep 3
    kill -INT $CLIENT1_PID
    kill -INT $CLIENT2_PID
    kill -INT $CLIENT3_PID

    killall cluster
    for a in 28 38 39 43 49; do
      ssh 128.36.232.$a 'killall mediator';
    done
    for a in 26 27 29 34 35 37 42 44 46 50; do
      ssh 128.36.232.$a 'killall preproc';
    done
    for a in 5 6 7 8 9 12 14 15 16 18; do # 19 20 21 22 24 26 27 28 29 34; do
      ssh 128.36.232.$a "killall $DBNAME";
    done
  done
done
