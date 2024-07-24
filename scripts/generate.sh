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
  ./generator $(printf "%03d" $i)
  ./tc_validator $(printf "%03d" $i)
done
