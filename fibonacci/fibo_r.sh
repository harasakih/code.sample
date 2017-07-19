#!/bin/sh
#
#  Fibonacci number
#
#

# --+----1----+----2----+----3
# G.val
# --+----1----+----2----+----3

fibo_r () {
	local	f1
	local	f2
	local	f

	if [ $1 -eq 0 ] ; then
		f=0
	elif [ $1 -eq 1 ]; then
		f=1
	else
		f1=`fibo_r $(( $1 - 1 ))` 
		f2=`fibo_r $(( $1 - 2 ))`
		f=$(( $f1 + $f2 ))
	fi
	echo "$f"
}

	for (( i = 1; i <= $1; i++ ))
	do	
		t1=`date +%s`
		x=`fibo_r $i`
		t2=`date +%s`
		diff=$(( $t2 - $t1 ))
		echo  "Fibo(sh_r) $i time $diff $x"
	done
