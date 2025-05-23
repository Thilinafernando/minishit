#!/bin/bash

#tail -2 | awk '{print $1}'

for i in $(seq 1 2);
do
	echo -n "test $i "

	# Get the last two lines from the output of the program
	last_two_lines=$(./philo 4 200 200 205 3 | tail -n 2)
	
	# Extract the numbers (Philo <number>) from the last two lines
	num1=$(echo "$last_two_lines" | head -n 1 | awk '{print $2}')
	num2=$(echo "$last_two_lines" | tail -n 1 | awk '{print $2}')

	# Compare the two numbers
	if [[ "$num1" == "$num2" ]]; then
	echo "KO"
	else
	echo "OK"
	fi
done
