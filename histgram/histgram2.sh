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
Hist=( 0 0 0 0 0 )						# [0]-[4]
nHist=${#Hist[@]}						# 5
nHistM=$(( $nHist - 1 ))				# 4
SUM=0									# 合計数

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

	while read x
	do
		(( SUM++ ))
		if [ $x -lt ${Range[0]} ] ; then
			Hist[0]=$(( ${Hist[0]} + 1 ))
		elif [ ${Range[ $nRangeM ]} -le $x ] ; then
			Hist[$nRange]=$(( ${Hist[$nRange]} + 1 ))
		else
			for (( i=1; i<$nRange; i++ ))
			do
				ii=$(($i - 1 ))
				if [ ${Range[$ii]}  -le $x ] && [ $x -lt ${Range[$i]} ] ; then
					Hist[$i]=$(( ${Hist[$i]} + 1 ))
				fi
			done
		fi
	done < $FILE
}

do_disphist () {
	for (( i=0; i<$nHist; i++ ))
	do
		ii=$(( $i - 1 ))
		H=${Hist[$i]}
		R=$(( $H * 100 / $SUM ))

		if [ $i -eq 0 ] ; then
			printf "%7d - %7d:%3d:%d\n" 0             ${Range[$i]} $R $H
		elif [ $i -ge $nHistM ] ; then
			printf "%7d - %7d:%3d:%d\n" ${Range[$ii]} 0            $R $H
		else
			printf "%7d - %7d:%3d:%d\n" ${Range[$ii]} ${Range[$i]} $R $H
		fi
	done 
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
do_disphist
rm $TEMP
