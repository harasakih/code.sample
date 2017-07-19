# coding: UTF-8
#
import	sys
import	sqlparse

argvs	= sys.argv
argc	= len(argvs)

f	= open( argvs[1] )
sql	= f.read()
f.close

res=sqlparse.parse(sql)
stmt=res[0]

print sql
for	item in	stmt.tokens:
	if item.is_whitespace() != True :
		print	item.ttype, ":" , item.value
