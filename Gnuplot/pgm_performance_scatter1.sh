#!/bin/bash
#
# 全プログラムの、[IO-record vs CPU-TIME] の散布図を作成する
# IO-record,CPU-TIMEはサンプルデータの平均を使用する
#

# Gloval Vals
MYSQL=mysql                  # mysqlコマンド名
DBNAME=SMS                   # MySQL DB-name
DBUSER=nobody                # MySQL DB-user
DBPASS=nobody                # MySQL DB-password
DBHOST=192.168.0.112         # MySQL DB-host
#
TEMPTSV=""                   # TSVファイル名
TEMPSQL=""                   # SQLファイル名
TEMPGPL=""                   # GnuPlotファイル名

# --+----1----+----2----+----3----+----4
# SQLを実行し、結果をCSVに出力する
# ARGV
#  none
# --+----1----+----2----+----3----+----4
# 
do_mysql_tsv () {
# SQL-make
echo '
SELECT
  WK1.pgm,
  WK1.rec / WK1.cnt          as rec,
  WK1.cpu / WK1.cnt / 100    as cpu
FROM
(
  SELECT 
    pgm,
    sum(cnt)             as cnt,
    sum(sum_all_record)  as rec,
    sum(cpu)             as cpu
  FROM    pgm_performance 
  WHERE   (dm="DAY" or dm="ELS") AND
          sum_all_record is not NULL
  GROUP BY pgm
) AS WK1
ORDER BY cpu DESC
;
' > $TEMPSQL 

# Exec SQL : -N No-title
  cat $TEMPSQL | $MYSQL -N --user=$DBUSER --password=$DBPASS --host=$DBHOST $DBNAME > $TEMPTSV 
}

# --+----1----+----2----+----3----+----4
# Gnuplotでグラフ描画する
# ARGV
#  none
# --+----1----+----2----+----3----+----4
# 
do_gnuplot () {
# GPL-make
echo "
  set terminal aqua
  set xlabel \"IO-records\"
  set ylabel \"CPU(sec)\"

  set autoscale
#  set xrange [1:1e8]
#  set yrange [0.01:1000]
  set logscale x
  set logscale y
  set grid

  set title \"PGM performance. average of 1 jobstep performance\"

  plot \"$TEMPTSV\" using 2:3 with points title \"program\"
" > $TEMPGPL

# Exec Gnuplot
gnuplot $TEMPGPL

}

# --+----1----+----2----+----3----+----4
# MAIN
# --+----1----+----2----+----3----+----4

# TEMPFILE
rm /tmp/tsv.*
rm /tmp/sql.*
rm /tmp/gpl.*
TEMPTSV=`mktemp /tmp/tsv.XXXXXXXX`
TEMPSQL=`mktemp /tmp/sql.XXXXXXXX`
TEMPGPL=`mktemp /tmp/gpl.XXXXXXXX`

# SQL-DEFINE
do_mysql_tsv
do_gnuplot   
