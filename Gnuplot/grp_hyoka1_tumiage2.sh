#!/bin/bash
#
# グループ単位の時間帯別、CPU利用状況を積上げグラフ化する
# 
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
  grp,
  sum(cpu_00/100.), sum(cpu_01/100.), sum(cpu_02/100.), sum(cpu_03/100.), sum(cpu_04/100.),
  sum(cpu_05/100.), sum(cpu_06/100.), sum(cpu_07/100.), sum(cpu_08/100.), sum(cpu_09/100.),
  sum(cpu_10/100.), sum(cpu_11/100.), sum(cpu_12/100.), sum(cpu_13/100.), sum(cpu_14/100.),
  sum(cpu_15/100.), sum(cpu_16/100.), sum(cpu_17/100.), sum(cpu_18/100.), sum(cpu_19/100.),
  sum(cpu_20/100.), sum(cpu_21/100.), sum(cpu_22/100.), sum(cpu_23/100.)
FROM
  grp_hyoka1
WHERE (dm="DAY" or dm="ELS") AND
      month=6
GROUP BY grp
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
  set xlabel \"TIME(hour)\"
  set ylabel \"CPU(sec)\"

  set autoscale
  set xrange [-1:24]
#  set yrange [0.01:1000]
#  set logscale x
#  set logscale y
  set grid

  set style data histograms
  set style histogram rows
  set style fill solid 1.0 border -1
  set boxwidth 0.9 relative

  set title \"GRP performance.\"
  plot \"$TEMPTSV2\" \
      using  1, '' using  2, '' using  3, '' using  4, '' using  5,  \
  ''  using  6, '' using  7, '' using  8, '' using  9, '' using 10,  \
  ''  using 11, '' using 12, '' using 13, '' using 14, '' using 15,  \
  ''  using 16, '' using 17, '' using 18, '' using 19, '' using 20,  \
  ''  using 21, '' using 22, '' using 23, '' using 24, '' using 25   
" > $TEMPGPL
# 列方向は、grp名. ２５に満たない列は、wrnがでるが無視される

# Exec Gnuplot
gnuplot $TEMPGPL

}


do_reverse () {
cat $TEMPTSV | awk '
{
  if (max_nf < NF) max_nf = NF;
  max_nr = NR;
  for (x = 1; x <= NF; x++) { vector[x, NR] = $x; }
}
END {
  for (x = 1; x <= max_nf; x++) {
    if(x == 1) { printf("# "); }
    for (y = 1; y <= max_nr; y++) { printf("%s ", vector[x, y]) ; }
    printf("\n")
  }
}' > $TEMPTSV2
}

# --+----1----+----2----+----3----+----4
# MAIN
# --+----1----+----2----+----3----+----4

# TEMPFILE
rm /tmp/tsv.*
rm /tmp/tsv2.*
rm /tmp/sql.*
rm /tmp/gpl.*
TEMPTSV=`mktemp /tmp/tsv.XXXXXXXX`
TEMPSQL=`mktemp /tmp/sql.XXXXXXXX`
TEMPGPL=`mktemp /tmp/gpl.XXXXXXXX`
TEMPTSV2=`mktemp /tmp/tsv2.XXXXXXXX`


# SQL-DEFINE
do_mysql_tsv
do_reverse
do_gnuplot   
