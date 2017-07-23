#!/bin/bash
#
# JOBSTEP毎のCPU時間の、平均/最小/最大を表示する
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
# SQLを実行し、結果をTSVに出力する
# ARGV
#  none 
# --+----1----+----2----+----3----+----4
# 
do_mysql_tsv () {
# SQL-make
echo '
SELECT
  WK1.pgm,
  WK1.cpu_sum/WK1.cnt/100 as cpu_avg,
  WK1.cpu_min/100,
  WK1.cpu_max/100
FROM
(
  SELECT 
    pgm,
    sum(cnt)  as cnt ,
    sum(cpu)  as cpu_sum,
    min(cpu)  as cpu_min,
    max(cpu)  as cpu_max
  FROM    sms004_sms100 
  WHERE   (dm="DAY" or dm="ELS") 
  GROUP BY pgm
) as WK1
ORDER BY cpu_avg DESC
LIMIT 200
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
  set xlabel \"Program\"
  set ylabel \"CPU(sec)\"

  set autoscale
#  set xrange [1:1e8]
#  set yrange [0.01:1000]
#  set logscale x
  set logscale y
  set grid

  set title \"CPU-TIME per jobstep, avg,min,max from sms004_sms100\"

  plot     \"$TEMPTSV\" using  :(\$2) with points title \"avg\"
  replot   \"$TEMPTSV\" using  :(\$3) with points title \"min\"
  replot   \"$TEMPTSV\" using  :(\$4) with points title \"max\"
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
