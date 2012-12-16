#!/bin/sh

for n in `seq 100 100 4000`
do
    for s in `seq 6 6`
    do
        mkdir -p "$2/$n/$s"
        for i in `seq 100`
        do
            sleep 1
            ../generate_model "$1" "$n" "$s" "$2/$n/$s/$i"
            echo "$i"
        done
        echo "generated N = $n"
    done
done
