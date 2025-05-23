#!/bin/sh

x=1
try=0
perc=100
if [ $# -eq 5 ]; then
	cmd="./philo $1 $2 $3 $4 $5 | grep "died" | wc -l"
else
	echo "base test: 199 60 20 25 2";
	cmd="./philo 199 60 20 25 2 | grep "died" | wc -l"
fi
while [ $x -le 100 ]
do
	try=$( eval $cmd );
	if [ $try -eq 1 ]; then
		echo -n "D";
	else
		echo -n "#";
	fi
	perc=$(( $perc - $try ))
	x=$(( $x + 1 ))
done
echo "\n"$perc"% survival rate"
