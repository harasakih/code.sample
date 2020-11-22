#!/usr/bin/python
# -*- coding: utf-8 -*-
""" nodeinfにあってedgeにないnodeを検索する
未使用モジュールの検索
"""
import  sys
from Kakeizu import Kakeizu

def usage():
    """ usage """
    print "usage:"
    print "  %s PC|CP EDGE-FILE NODE-FILE" % sys.argv[0]

argc = len(sys.argv)
if argc != 4:
    usage()
    exit(1)

FLG =  sys.argv[1]
EDGE = sys.argv[2]
NODE = sys.argv[3]

KAKEIZU = Kakeizu()
KAKEIZU.edge_read(EDGE, FLG)
KAKEIZU.nodeinf_read(NODE, [1, 2], "who;rumor;no")

KAKEIZU.sison_maketree(1, "p", "kiten")


cnt = 0
otfile = open("./otdir/Misiyou.txt", "w")
otfile2 = open("./otdir/Misiyou.Ommit.txt", "w")
#
for key, val in KAKEIZU.nodeinf.items():
    (who, rumor, no, dmy) = val.split(';')
    if key in KAKEIZU.childs:
        continue
    if key in KAKEIZU.parents:
        continue

    cnt = cnt + 1
    print >> otfile, who, rumor

otfile.close()
otfile2.close()
