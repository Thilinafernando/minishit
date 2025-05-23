#!/usr/bin/bash

max=0;

# 5 TEST
for i in $(seq 1 200)
do
	ARG=$(shuf -i 0-2147483640 -n 5 | tr '\n' ' ')
	moves=$(./push_swap $ARG | wc -l)
	if [ "$moves" -ge "$max" ]; then
        max=$moves
    fi
done
echo "MAX[5  ]=$max";

# 10 TEST
for i in $(seq 1 200)
do
	ARG=$(shuf -i 0-2147483640 -n 10 | tr '\n' ' ')
	moves=$(./push_swap $ARG | wc -l)
	if [ "$moves" -ge "$max" ]; then
        max=$moves
    fi
done
echo "MAX[10 ]=$max";

max=0;

# 100 TEST
for i in $(seq 1 100)
do
	ARG=$(shuf -i 0-2147483640 -n 100 | tr '\n' ' ')
	moves=$(./push_swap $ARG | wc -l)
	if [ "$moves" -ge "$max" ]; then
        max=$moves
    fi
done
echo "MAX[100]=$max";

max=0;

# 500 TEST
for i in $(seq 1 25)
do
	ARG=$(shuf -i 0-2147483640 -n 500 | tr '\n' ' ')
	moves=$(./push_swap $ARG | wc -l)
	if [ "$moves" -ge "$max" ]; then
        max=$moves
    fi
done
echo "MAX[500]=$max";
