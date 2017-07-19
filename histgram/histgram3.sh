#!/bin/bash
#
#	ファイルサイズのヒストグラム
#

# --+----1----+----2----+----3----+----4----+----5----+----6----+----7
#	G.vals
# --+----1----+----2----+----3----+----4----+----5----+----6----+----7
#
BASEDIR=""								# 基準デリレクトリ
Range=( 100 1000 10000 100000 )			# [0]-[3]
nRange=${#Range[@]}						# 4
nRangeM=$(( $nRange - 1 ))				# 3
HIST=""

# --+----1----+----2----+----3----+----4----+----5----+----6----+----7
#	Functions
# --+----1----+----2----+----3----+----4----+----5----+----6----+----7
#
do_fszlist () {
	local   OTFILE
	local	BASE
	
	BASE="$1"
	OTFILE="$2"
	ls -l -R $BASE 2>&1 2>/dev/null | grep -E "^[\-][rwx\-]{9}" | awk '{ print $5 }' | sort -n > $OTFILE
}

do_makehist () {
	local	FILE
	FILE="$1"

	HIST=0
	keyP=0
	keyN=0

	while read x
	do
		if [ $x -lt ${Range[0]} ] ; then
			keyN=0
		elif [ ${Range[ $nRangeM ]} -lt $x ] ; then 
			keyN=$nRange
		else
			for (( i=1; i<$nRange; i++ ))
			do
				ii=$(( $i - 1 ))
				if [ ${Range[$ii]} -le $x ] && [ $x -lt ${Range[$i]} ] ; then
					keyN=$i
				fi
			done
		fi	

		if [ $keyN != $keyP ] ; then
			printf "%7d:%d\n" ${Range[$keyP]} $HIST
			keyP=$keyN
			HIST=1
		else
			keyP=$keyN
			(( HIST++ ))
		fi
	done < $FILE
	if [ $keyP -lt $nRange ]; then
		printf "%7d:%d\n" ${Range[$keyP]} $HIST
	else
		printf "%7d:%d\n" 0 $HIST
	fi
}

do_usage() {
	echo "usage :"
	echo ${0##*/} "BaseDIR"
}

# --+----1----+----2----+----3----+----4----+----5----+----6----+----7
#	main
# --+----1----+----2----+----3----+----4----+----5----+----6----+----7
#
if [ $# -ne 1 ] ; then
	do_usage
	exit 1
fi

BASEDIR="$1"
TEMP=`mktemp /tmp/histgram.$$`

do_fszlist $BASEDIR $TEMP
do_makehist $TEMP
rm $TEMP
