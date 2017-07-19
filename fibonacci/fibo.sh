#!/bin/sh
#
fibo_r () {
	local	f1
	local	f2
	local	f

	if [ $1 -eq 0 ] ; then
		f=0
	elif [ $1 -eq 1 ]; then
		f=1
	else
		f1=`./fibo.sh $(( $1 - 1 ))` 
		f2=`./fibo.sh $(( $1 - 2 ))`
		f=$(( $f1 + $f2 ))
	fi
	echo "$f"
}

fibo_r $1
