#!/bin/sh 
#telling POSIX to run script with sh

if [ $# -lt 2 ]; then #-lt stands as less than wrong usage
	echo "Usage is ./perf_check <exe> <times>";
	exit 1;
fi

test_exe="$1" #script for checking first argv
tests="$2" #test numbers second argv

min="" #min and max for time of execution
max=""

sum=0 #sum is for finding average of special approach of solution
i=1

while [ $i -le $tests ]; do #-le means less equal	
	elapsed=$(/usr/bin/time -f "%e" "$test_exe" 2>&1 >/dev/null) #storing elapsed time using float number ariphmetics
	#usr bin time for runing time it default puts res to stderr so we redir it to stdout %e for format 0.123 to 123 miliseconds as AI SAYS)
	echo "Run $i: $elapsed sec" #printing info about each test time
	if [ -z "$min" ]; then #if our min and max are 0s so not inited putting elapsed time for first time to compare in future
    		min=$elapsed
    		max=$elapsed
	fi
	# if this run wass less than the minimal minimal become current
	is_less=$(echo "$elapsed < $min" | bc)
	if [ "$is_less" -eq 1 ]; then
		min=$elapsed
	fi
	#So do for max
	is_greater=$(echo "$elapsed > $max" | bc)
	if [ "$is_greater" -eq 1 ]; then
    		max=$elapsed
	fi
	sum=$(echo "$sum + $elapsed" | bc) #summing results for average
	i=$((i + 1)) #doing next test
done 
average=$(echo "scale=6; $sum / $tests" | bc) #getting average

#printing times
echo "----------------------------"
echo "Min time: $min sec"
echo "Max time: $max sec"
echo "Average: $average sec"

