#!/bin/bash

if [[ -z $2 ]]; then
    l=0
    cnt=$(ls -1 $SCHED_TC_FOLDER | wc -l)
    r=$((cnt-1))
elif [[ -z $3 ]]; then
    l="$2"
    r="$2"
else
    l="$2"
    r="$3"
fi

for i in $(seq $l $r);
do
    schedulers/"$1"/sched $(printf "%03d" $i)
    # schedule_validator/validator $1 $(printf "%03d" $i)
    # schedule_viewer/main.py $1 $(printf "%03d" $i)
done
