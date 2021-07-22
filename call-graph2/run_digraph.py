#!/usr/local/bin/python3
# -*- coding: utf-8 -*-
#!/usr/bin/python
""" GvKakeizuを利用して、edgeのネットワーク図を作成する
"""
import sys
from Gvkakeizu import GvKakeizu


def usage():
    """ usage """
    print( "usage:" )
    print( "  %s PC|CP EDGE" % sys.argv[0] )

# --+----1----+----2----+----3
# main
# --+----1----+----2----+----3
argc = len(sys.argv)

if argc != 3:
    usage()
    exit(1)

flg = sys.argv[1]
csvfile = sys.argv[2]

if flg != "CP" and flg != "PC":
    usage()
    exit(1)

GVKAKE = GvKakeizu()

GVKAKE.edge_read(csvfile, flg)
GVKAKE.gvnetwork_pdf(GVKAKE.edges, wkdir="./otdir/", gvformat='pdf')
