#!/bin/bash
#
# program名を指定し、[IO-record vs CPU-TIME]の散布図を作成する
# 

# Gloval Vals
MYSQL=mysql                  # mysqlコマンド名
DBNAME=SMS                   # MySQL DB-name
DBUSER=nobody                # MySQL DB-user
DBPASS=nobody                # MySQL DB-password
DBHOST=192.168.0.112         # MySQL DB-host

# --+----1----+----2----+----3----+----4
# SQLを実行し、結果をTSVに出力する
# ARGV
#  $1    : Program Name
#  $2    : TSVファイル名
# --+----1----+----2----+----3----+----4
# 
do_mysql_tsv () {
local PGM="$1"
local OUTF="$2"

SQL="
SELECT 
  sum_all_record,
  cpu/100 as cpu
FROM    pgm_hyoka1 
WHERE   pgm=\"$PGM\" AND
        sum_all_record is not NULL AND
		day=1
;
"
# Exec SQL : -N No-title
  echo $SQL | $MYSQL -N --user=$DBUSER --password=$DBPASS --host=$DBHOST $DBNAME > $OUTF
}

# --+----1----+----2----+----3----+----4
# Gnuplotでグラフ描画する
# ARGV
#  $1    : Graph title
#  $2    : TSVファイル名
# --+----1----+----2----+----3----+----4
# 
do_gnuplot () {
  local PGM="$1"
  local TSV="$2"

# Exec Gnuplot 
gnuplot << _EOT_
#  set terminal aqua
#  set terminal canvas 
#  set output "sample1.html"
  set terminal svg
  set output "sample1.svg"

  set xlabel "Record Count"
  set ylabel "CPUTIME(sec)"

  # set autoscale
  set xrange [1:1e8]
  set yrange [0.01:1000]
  set logscale x
  set logscale y
  set grid

  set title "Program performance. Historical data"

  plot "$TSV" title "$PGM"
  replot 1e-5*x with line
_EOT_

}

# --+----1----+----2----+----3----+----4
# MAIN
# --+----1----+----2----+----3----+----4

# TEMPFILE
rm /tmp/tsv.*
TEMPFILE=`mktemp /tmp/tsv.XXXXXXXX`

# 
PGM=$1
do_mysql_tsv "$PGM" "$TEMPFILE"
do_gnuplot   "$PGM" "$TEMPFILE"

