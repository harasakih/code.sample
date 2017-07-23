#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
#	ＳＭＳエクセルを読込む
#
'''
データは６列目から
1	A	項番
2 *	B	ＪＯＢ名
3	C	ＪＯＢクラス
4	D	読込開始日付
5	E	読込開始時刻(時:分:秒)
6 *	F	JＯＢ実行時間(時:分:秒.ﾐﾘ秒)
7	G	ＪＯＢ QUEUE滞留時間(時:分:秒.ﾐﾘ秒)
8 *	H	ＪＯＢ開始日付
9 *	I	ＪＯＢ開始時刻(時:分:秒)
10*	J	ＪＯＢ終了日付
11*	K	ＪＯＢ終了時刻(時:分:秒)
12	L	終了状態
13*	M	CPU使用時間(時:分:秒.ﾐﾘ秒)
14*	N	アクションCPU使用時間(時:分:秒.ﾐﾘ秒)
15*	O	タスクCPU使用時間(時:分:秒.ﾐﾘ秒)"
16	P	空間優先順位
17*	Q	ＥＸＣＰ回数
18	R	サービス量
19*	S	SAMファイルの入力件数合計
20*	T	SAMファイルの出力件数合計
21*	U	SAMファイルの入出力件数合計
'''


import	pyExcelerator
import	sys
import	os
import	csv
import	datetime
import	locale
#
# SR-DEFINE
#

# ------------------------------------------------
# Name	: sms_excel2csv
# Args
#	1	xlsName			: EXCEL input file name
#	2	targetsheetName	: sheetname to be parsed
#	3	csvName			: CSV output file name
# Return
#	0	none
# ------------------------------------------------
def	sms_excel2csv(xlsName, targetsheetName, csvName):
	sheets	= pyExcelerator.parse_xls(xlsName)
#
#	writer,csvともprintと比べて遅いが大差ないので、CSVを使う
#	print << writer << csv
#
#	otfile	= open(csvName, 'w')					# printでファイル出力するとき
	writer	= csv.writer(file(csvName, 'wb'))		# csvで出力するとき

	for(sheetName, sheet)	in sheets:
#		print "SheetName: %s" % sheetName
		if sheetName == targetsheetName :
			keys	= sorted(sheet.keys())
			(max_row, max_col) = max(keys)
#D
#			print len(keys)
#			exit(-1)

			for row in range(5 , max_row) :
				if (row % 500) == 0 :
					print "row is %d/%d" % (row, max_row)

				smsJobname	=	sheet[(row,1)]
				smsJobtime	=	sheet[(row,5)]
				smsSdate	=	sheet[(row,7)]
				smsStime	=	sheet[(row,8)]
				smsEdate	=	sheet[(row,9)]
				smsEtime	=	sheet[(row,10)]

				smsCpuall	=	sheet[(row,12)]
				smsCpuaction	=	sheet[(row,13)]
				smsCputask	=	sheet[(row,14)]

# EXCP情報がないときはゼロ
				smsExcp		=	sheet[(row,16)]
				if (row,18) in keys :
					smsInput	=	sheet[(row,18)]
					smsOutput	=	sheet[(row,19)]
					smsTotal	=	sheet[(row,20)]
				else :
					smsInput	=	0
					smsOutput	=	0
					smsTotal	=	0
# 時刻表記を秒に変換
				smsJobtime_sec		=	time2sec( smsJobtime )
				smsCpuall_sec		=	time2sec( smsCpuall )
				smsCpuaction_sec	=	time2sec( smsCpuaction )
				smsCputask_sec		=	time2sec( smsCputask )

#	csvの例
				writer.writerow( \
				[smsJobname	, smsSdate	 , smsStime	, smsEdate	, smsEtime	, \
				smsCpuall	, smsCpuaction , smsCputask	, \
				smsExcp		, smsInput	, smsOutput	, smsTotal, \
				smsJobtime_sec, smsCpuall_sec, smsCpuaction_sec, smsCputask_sec ])

# printでファイル出力
#				print  >> otfile , \
#				"%s,%s,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%f,%f,%f,%f" % \
#				(smsJobname	, smsSdate	 , smsStime	, smsEdate	, smsEtime	, \
#				smsCpuall	, smsCpuaction , smsCputask	, \
#				smsExcp		, smsInput	, smsOutput	, smsTotal, \
#				smsJobtime_sec, smsCpuall_sec, smsCpuaction_sec, smsCputask_sec)
# f-close
#	otfile.close		# printの時

# ------------------------------------------------
# Name	: time2sec
#	time-format(hh:mm:ss.SS) to sec-vale
#
# Args
#	1	xlsName			: EXCEL input file name
#	2	targetsheetName	: sheetname to be parsed
#	3	csvName			: CSV output file name
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

#
# 実行
#

retval	=	0
stime	=	datetime.datetime.today()
print	"JOB-start " + stime.strftime("%x %X")

if len(sys.argv) < 3:
	print "python chiginsms.py EXCEL-NAME"
	retval	=	-1
else:
	InputPath	=	sys.argv[1]
	OutputPath	=	sys.argv[2]

	flists	=	os.listdir(InputPath)
	for flist in flists :
#		print flist
		inxls	=	InputPath + '/' + flist
		(ot_base, ot_ext)	= os.path.splitext( flist )
		otcsv	=	OutputPath + '/' + ot_base + '.csv'
		print "%s -> %s" % (inxls, otcsv)

		sms_excel2csv( inxls, u"バッチジョブCPU・実行時間明細", otcsv )
	retval	=	0
#

etime	=	datetime.datetime.today()
print	"JOB-end   " + etime.strftime("%x %X")

print	"jobs %s to %s, return is %d" % \
		(stime.strftime("%x %X"), etime.strftime("%x %X"), retval)

sys.exit( retval )
