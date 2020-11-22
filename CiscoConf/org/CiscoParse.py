#!/usr/local/bin/python
# -*- coding: utf-8 -*-
##!/usr/bin/python
##!/usr/local/bin/python

from CTextFsm import CTextFsm
import pprint
import ipaddress
import os
import sys

class CiscoParse(CTextFsm):
    """docstring for test."""

    def __init__(self):
        CTextFsm.__init__(self)
        pass

if __name__ == '__main__':
    basename = os.path.basename(sys.argv[0])
    dirname = os.path.dirname(sys.argv[0])
    #   CTextFsm.Ntc_Dir = '/usr/lib/python2.7/site-packages/ntc_templates-1.1.0-py2.7.egg/ntc_templates/templates/'
    CTextFsm.Ntc_Dir = dirname + '/ntc_templates/'
    header = None

    templfname = sys.argv[1]
    confname = sys.argv[2]
    templfname = CTextFsm.Ntc_Dir + templfname

    ciscoparse = CiscoParse()
    ciscoparse.dbgprint(3, 'confcsv-templfname', os.path.basename(templfname))
    ciscoparse.dbgprint(3, 'confcsv-confname  ', os.path.basename(confname))

    header = None
    confobj   = ciscoparse.read_conf(confname)
    parsedobj = ciscoparse._parse(confobj, templfname)
    ciscoparse.csvprint(parsedobj, header)
