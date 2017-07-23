#!/usr/bin/env python
# -*- coding: utf-8 -*-

#	DB		: SMS
#	TABLE	: sms004_sms100
#
#	(record, cpu, elaps)の３Ｄグラフを作成する
#

import	MySQLdb
import	Gnuplot
import	sys
import	os

argc		=	len(sys.argv)

DB_name		= "SMS"
DB_user		= "nobody"
DB_passwd	= "nobody"
Host		= "192.168.0.112"

cgi_table	= "sms004_sms100"
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

# plain file output
# fw = open(cgi_pgm + '.txt', 'w')

# exec sql
grp_title	=  "PGM[" + cgi_pgm + "]"
grp_title	+= " DM[" + cgi_dm + "]"
grp_title	+= " YEAR[" + cgi_year + "]"
grp_title	+= " MON[" + cgi_month + "]"
grp_title	+= " DAY[" + cgi_day + "]"
grp_title	+= " GRP[" + cgi_grp + "]"


sql = 'select sum_all_record, cpu, \
	stp_s_year, stp_s_day, stp_s_time, \
	stp_e_year, stp_e_day, stp_e_time  \
 from sms004_sms100 \
 where sum_all_record is not null\
 and pgm="' + cgi_pgm + '"'


print "SQL:" + sql
cur.execute( sql )


# initiate d, plot-data for gnuplot
d = []

# cursor fetch
rows = cur.fetchall()
for row in rows:
	rec_cnt	=	row[0]			# total-records
	cpu		=	row[1]/100.0	# cpu-time(sec)

	year1	=	row[2]			# step-start-year
	day1	=	row[3]			#            day
	time1	=	row[4]			#            time(1/100 msec)

	year2	=	row[5]			# step-start-year
	day2	=	row[6]			#            day
	time2	=	row[7]			#            time(1/100 msec)

	grp_x	=	row[0]
	grp_y	=	row[1]

	min1	= ((year2 - year1)*365 + (day2 - day1))*24*60
	min2	= (time2 - time1)/100.0/60
	grp_z	=	min1 + min2

	d.append( [grp_x, grp_y, grp_z] )
	dmy_str = '%f %f %f \n' % (grp_x, grp_y, grp_z)	
#	print dmy_str	
#	fw.write( dmy_str )	

cur.close()
connect.close()
# fw.close()


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
gp('set zlabel "ELAPS(min)"')

# x-yの描画範囲を指定する 
# gp('set xrange [1:100000000]')
# gp('set yrange [0.01:1000]')
# gp('set logscale x')
# gp('set logscale y')

# AXIS 
#   [xy]tics メモリ
#   gp('set xtics 10')
gp('set autoscale')


# グリッドを表示
gp('set grid')

# 重ね合わせをするので、ますはヌルへ第一描画
if	os.name	==	"posix"	or \
	os.name	==	"mac" :
	gp('set output "/dev/null"')		# linux,mac
else :
	gp('set output "NUL"')				# windows


# 第二描画
if	output_device	<> "default" :
	output_fname	=	cgi_pgm + '.' + output_device
	gp('set output "' + output_fname + '"')
	print	output_fname

# with points で散布図として描画
gp('set style data points')
gp.splot(d)

print 'hit a key'
raw_input('>>> ')

