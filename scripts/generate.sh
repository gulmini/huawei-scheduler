#!/bin/bash

if [[ -z $1 ]]; then
    l=0
    cnt=$(ls -1 $SCHED_GEN_FOLDER | wc -l)
    r=$((cnt-1))
elif [[ -z $2 ]]; then
    l="$1"
    r="$1"
else
    l="$1"
    r="$2"
fi

for i in $(seq $l $r);
do
    testcase_generator/generator $(printf "%03d" $i)
    testcase_generator/validator $(printf "%03d" $i)
    #graph_viewer/main.py $(printf "%03d" $i)
done
