#!/usr/local/bin/python3
# -*- coding: utf-8 -*-
""" edgeにあってnodeinfにないものを検索する
不明モジュールの検索
"""
import  sys
from Kakeizu import Kakeizu

def usage():
    """ usage """
    print( "usage:" )
    print( "  %s PC|CP EDGE-FILE NODE-FILE" % sys.argv[0] )

argc = len(sys.argv)
if argc != 4:
    usage()
    exit(1)

FLG = sys.argv[1]
EDGE = sys.argv[2]
NODE = sys.argv[3]

KAKEIZU = Kakeizu()
KAKEIZU.edge_read(EDGE, FLG)
KAKEIZU.nodeinf_read(NODE, [1, 2], 'who;rumor;no')

otfile = open("./otdir/Unknown.txt", "w")
otfile2 = open("./otdir/Unknown.Ommit.txt", "w")

unknown = {}
for key in KAKEIZU.childs:
    if key in KAKEIZU.nodeinf:
        continue
    else:
        unknown[key] = 1

for key in KAKEIZU.parents:
    if key in KAKEIZU.nodeinf:
        continue
    else:
        unknown[key] = 1

for key in sorted(unknown.keys()):
    otfile.write( "%s\n" % key )
    if key in KAKEIZU.sison:
        otfile.write( "  mychild :%s\n" % KAKEIZU.sison[key] )
    if key in KAKEIZU.sosen:
        otfile.write( "  myparent:%s\n" % KAKEIZU.sosen[key] )

otfile.close()
otfile2.close()
