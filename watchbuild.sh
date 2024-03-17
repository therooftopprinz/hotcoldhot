#!/usr/bin/bash

while :
do
    inotifywait -e modify -r $1
    make -j
    pkill -9 hotcoldhot
    nohup ./hotcoldhot &
done