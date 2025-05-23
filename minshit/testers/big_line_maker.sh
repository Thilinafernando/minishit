#!/bin/bash

n=4096;

if [[ $# != 0 ]]; then
	n=$1;
fi
echo -n $n:
for n in $(seq 1 $n); do
	echo -n "#";
done
