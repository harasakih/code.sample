#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Tips:
#        argv[0] argv[1] ; argc = 2
# python *.py    DATE
#


import	MySQLdb
import	Gnuplot
import	os
import	sys

# ------------------------------------------------
#	SQL
# ------------------------------------------------
def	exec_sql():
	# connect & cursor
	connect	= MySQLdb.connect(db=DB_name, host=Host, port=3306, \
		user=DB_user, passwd=DB_passwd)
	cur		= connect.cursor()

	# exec sql
	sql		= 'SELECT smsEtime, smsCpuall_sec FROM ' + cgi_table
	sql		+= ' WHERE smsEdate = "' + cgi_date + '"'
	sql		+= ' ORDER BY smsEtime'

	print "SQL:" + sql
	cur.execute( sql )

	# cursor fetch
	rows = cur.fetchall()

	sum_cpusec	=	0
	for row in rows:
		etime		=	row[0]		# end-time hh:mm:ss
		etime_min	=	time2sec( etime )  / 60

		cputime_sec	=	row[1]		# cputime as seond
		sum_cpusec	+=	cputime_sec	# integrate cputime_sec

		grp_x	= etime_min			# total-records
		grp_y	= sum_cpusec / 60	# cpu-time (sec)
		
		gdata.append( [grp_x, grp_y] )
	#	print	grp_x, grp_y

	cur.close()
	connect.close()

# ------------------------------------------------
# Name	: time2sec
#	time-format(hh:mm:ss.SS) to sec-vale 
# 
# Args
#	1	hh:mm:ss.SS format time-value
# Return
#	0	time by second
# ------------------------------------------------
def	time2sec( t ):
	
	if len(t) == 11:
		retval	=	(int(t[0:2])*60 + int(t[3:5]))*60 + float(t[6:11])
		return retval

	if len(t) == 8:
		retval	=	(int(t[0:2])*60 + int(t[3:5]))*60 + float(t[6:8])
		return retval




# ----------------------------------------------------------
#	Graph
# ----------------------------------------------------------
def	exec_gnuplot():
	gp = Gnuplot.Gnuplot()

	# OUTPUT-device
	#   set termilal png/ps(postscript)/x11
	if	output_device <> "default" :
		gp('set terminal ' + output_device)


	# TITIE&LABEL
	gp('set title "'+ cgi_date + '"')
	gp('set xlabel "TIME(min)"')
	gp('set ylabel "CPUTIME(min)"')


	# AXIS 
	#   [xy]tics メモリ
	gp('set xtics 60')
	gp('set ytics 60')
	gp('set autoscale')

	# x-yの描画範囲を指定する 
	#gp('set xrange [0:1440]')
	#gp('set yrange [0:2400]')

	# グリッドを表示
	gp('set grid')

	# 第一描画
	gp('set style data lines')
	gp.plot( gdata )


	raw_input('hit any key >>')

# ----------------------------------------------------------
#	Main
# ----------------------------------------------------------
argc		=	len(sys.argv)

DB_name		= "CHIGIN"
DB_user		= "nobody"
DB_passwd	= "nobody"
Host		= "192.168.0.210"

cgi_table	= "sms"
if	argc > 1 :
	cgi_date	=	sys.argv[1]
else :
	usage()

if	argc > 2 :
	output_device	=	sys.argv[2]
else :
	output_device	=	"default"

# initiate graph-data
gdata	=	[]

exec_sql()
exec_gnuplot()


