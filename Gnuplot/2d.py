#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Tips:
#        argv[0] argv[1]  argc = 2
# python 2d.py MODULE
#


import	MySQLdb
import	Gnuplot
import	os
import	sys

argc		=	len(sys.argv)

DB_name		= "SMS"
DB_user		= "nobody"
DB_passwd	= "nobody"
Host		= "192.168.0.210"

cgi_table	= "pgm_hyoka1"
cgi_dm		= "%"
cgi_year	= "%"
cgi_month	= "%"
cgi_day		= "%"
cgi_grp		= "%"
cgi_pgm		= sys.argv[1]


if	argc > 2 :
	output_device	=	sys.argv[2]
else :
	output_device	=	"default"


# connect & cursor
connect	= MySQLdb.connect(db=DB_name, host=Host, port=3306, user=DB_user, passwd=DB_passwd)
cur		= connect.cursor()

# exec sql
sql		= 'select sum_all_record, cpu from ' + cgi_table
sql		+= ' where sum_all_record is not null'
sql		+= ' AND dm = "' + cgi_dm + '"'
sql		+= ' AND job_s_year = ' + cgi_year
sql		+= ' AND month in(' + cgi_month + ')'
sql		+= ' AND day in(' + cgi_day + ')'
sql		+= ' AND grp like "' + cgi_grp + '"'
sql		+= ' AND pgm ="' + cgi_pgm + '"'

grp_title	=  "PGM[" + cgi_pgm + "]"
grp_title	+= " DM[" + cgi_dm + "]"
grp_title	+= " YEAR[" + cgi_year + "]"
grp_title	+= " MON[" + cgi_month + "]"
grp_title	+= " DAY[" + cgi_day + "]"
grp_title	+= " GRP[" + cgi_grp + "]"


sql = 'select sum_all_record, cpu \
 from pgm_hyoka1 \
 where sum_all_record is not null \
 and pgm="' + cgi_pgm + '"'

print "SQL:" + sql
cur.execute( sql )


# initiate d, plot-data for gnuplot
d = []

# cursor fetch
rows = cur.fetchall()
for row in rows:
	grp_x	= row[0]		# total-records
	grp_y	= row[1]/100.0	# cpu-time (sec)
	
	d.append( [grp_x, grp_y] )
#	print	grp_x, grp_y

cur.close()
connect.close()

# ----------------------------------------------------------
# Graph
# ----------------------------------------------------------
gp = Gnuplot.Gnuplot()

# OUTPUT-device
#   set termilal png/ps(postscript)/x11
if	output_device <> "default" :
	gp('set terminal ' + output_device)


# TITIE&LABEL
gp('set title "' + grp_title + '"')
gp('set xlabel "Record Count"')
gp('set ylabel "CPUTIME(sec)"')


# AXIS 
#   [xy]tics メモリ
#   gp('set xtics 10')
#   gp('set autoscale')

# x-yの描画範囲を指定する 
gp('set xrange [1:100000000]')
gp('set yrange [0.01:1000]')
gp('set logscale x')
gp('set logscale y')

# グリッドを表示
gp('set grid')


# 重ね合わせをするので、ますはヌルへ描画
if	os.name	==	"posix"	or \
	os.name	==	"mac" :
	gp('set output "/dev/null"')		# linux,mac
else :
	gp('set output "NUL"')				# windows

# with points で散布図として,第一描画
gp('set style data points')
gp.plot(d)

# 第二描画
if	output_device	<> "default" :
	output_fname	=	cgi_pgm + '.' + output_device
	gp('set output "' + output_fname + '"')
	print	output_fname

gp.replot('1e-5*x with line')

raw_input('hit any key >>')
