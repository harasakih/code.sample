#!/usr/bin/python
# -*- coding: utf-8 -*-
##!/usr/bin/python
##!/usr/local/bin/python

from CTextFsm import CTextFsm
from CTextFsm import Aclobj
from CTextFsm import MyIpAddress

import pprint
import ipaddress
import os
import sys


class test(CTextFsm):
    """docstring for test."""

    def __init__(self):
        CTextFsm.__init__(self)
        pass


    def ut_acl2(self):
        print 'ut_acl2 aclname-NG'
        self.ut_ismatch_acl2(True, 'WAN-out', 'ip    10.246.0.1 any 10.246.0.1 any', net4obj)
        self.ut_ismatch_acl2(False, 'http', 'ip    10.246.0.1 any 10.246.0.1 any', net4obj)
        pass

    def ut_acl1(self):
        # 基本的な正常パターン
        print 'ut_acl1 prptocol'
        self.ut_ismatch_acl(True, 'WAN-out eigrp 10.246.0.1 any 10.246.0.1 any', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out icmp  10.246.0.1 any 10.246.0.1 any', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out udp   192.168.0.120 any 10.246.0.1 123', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out udp   10.246.0.1 any 10.246.0.1 123', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out tcp   0.0.0.0    any 8.8.8.8 established', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out tcp   10.246.0.1 any 10.246.0.1 23', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out tcp   10.246.0.1 any 10.246.0.1 22', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out ip    10.246.0.1 any 10.246.0.1 any', net4obj)
        ## src-ip
        print 'ut_acl1 src-ip'
        self.ut_ismatch_acl(True, 'WAN-out ip    10.246.0.1 any 10.246.0.1 any', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out ip    10.246.127.255 any 10.246.0.1 any', net4obj)
        self.ut_ismatch_acl(False, 'WAN-out ip    10.246.128.0   any 10.246.0.1 any', net4obj)
        ## dst-ip
        print 'ut_acl1 dst-ip'
        self.ut_ismatch_acl(True, 'WAN-out ip    10.246.0.1 any 10.246.0.1 any', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out ip    10.246.0.1 any 10.246.255.255 any', net4obj)
        self.ut_ismatch_acl(False, 'WAN-out ip    10.246.0.1 any 10.247.0.0 any', net4obj)
        self.ut_ismatch_acl(False, 'WAN-out ip    10.246.100.1 any 100.100.100.100 9999', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out tcp   10.246.100.1 any 100.100.100.100 9999', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out tcp   10.246.100.1 any 100.100.100.200 9999', net4obj)
        # wildcard mask
        print 'ut_acl1 src-wild'
        self.ut_ismatch_acl(True, 'WAN-out ip    10.246.0.0   any 10.246.0.0    any', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out ip    10.246.127.0 any 10.246.0.0    any', net4obj)
        self.ut_ismatch_acl(False, 'WAN-out ip    10.246.128.0 any 10.246.0.0    any', net4obj)
        self.ut_ismatch_acl(False, 'WAN-out ip    10.246.255.0 any 10.246.0.0    any', net4obj)
        print 'ut_acl1 dst-wild'
        self.ut_ismatch_acl(True, 'WAN-out ip    10.246.0.0   any 192.168.1.0   any', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out ip    10.246.0.0   any 192.168.1.255 any', net4obj)
        self.ut_ismatch_acl(False, 'WAN-out ip    10.246.0.0   any 192.168.3.0   any', net4obj)

        # tcp,udpがipで制御
        print 'ut_acl1 contolled by ip'
        self.ut_ismatch_acl(True, 'WAN-out ip    10.246.2.1   any 192.168.1.0   any', net4obj)
        self.ut_ismatch_acl(False, 'WAN-out ip    10.246.6.1   any 192.168.3.0   any', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out tcp   10.246.3.1   any 172.32.0.0    any', net4obj)
        self.ut_ismatch_acl(False, 'WAN-out tcp   10.246.3.1   any 172.128.0.0   any', net4obj)
        # PORT
        print 'ut_acl1 port'
        self.ut_ismatch_acl(True, 'WAN-out tcp   10.246.2.0 any 172.16.0.0 100', net4obj)
        self.ut_ismatch_acl(False, 'WAN-out tcp   10.246.2.0 any 172.16.0.0 150', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out tcp   10.246.4.0 any 10.0.0.0 1000', net4obj)
        self.ut_ismatch_acl(True, 'WAN-out tcp   10.246.4.0 any 10.0.0.0 3000', net4obj)
        self.ut_ismatch_acl(False, 'WAN-out tcp   10.246.4.0 any 10.0.0.0 300', net4obj)
        # established
        print 'ut_acl1 established'
        self.ut_ismatch_acl(True, 'WAN-out tcp   0.0.0.0 any 255.255.255.255 established', net4obj)
        # Standard-acl
        print 'ut_acl1 standard-acl'
        self.ut_ismatch_acl(True, '5       ip    192.168.1.1 any 0.0.0.0 any', net4obj)
        self.ut_ismatch_acl(True, '5       ip    192.168.3.255 any 0.0.0.0 any', net4obj)
        self.ut_ismatch_acl(False, '5       ip    192.168.4.0 any 0.0.0.0 any', net4obj)
        self.ut_ismatch_acl(True, '5       icmp  192.168.1.1 any 0.0.0.0 any', net4obj)
        self.ut_ismatch_acl(False, '5       icmp  9.9.9.9 any 8.8.8.8 any', net4obj)
        self.ut_ismatch_acl(True, '6       ip    192.168.0.1 any 8.8.8.8 any', net4obj)
        self.ut_ismatch_acl(False, '6       eigrp 9.9.9.9 any 8.8.8.8 any', net4obj)
        self.ut_ismatch_acl(False, '6       hoge  9.9.9.9 any 8.8.8.8 any', net4obj)
        #
        #   Error試験
        #
        # エラー系　: aclname not match
        self.ut_ismatch_acl(False, 'WAN-OUT tcp   0.0.0.0 any 255.255.255.255 established', net4obj)
        # エラー系　: protocol
        self.ut_ismatch_acl(False, 'WAN-out ipv6  0.0.0.0 any 255.255.255.255 established', net4obj)
        # エラー系　: src-ip ---> exception
        self.ut_ismatch_acl(False, 'WAN-out ip    0.0.0.256 any 255.255.255.255 established', net4obj)
        # エラー系　: dst-ip ---> exception
        self.ut_ismatch_acl(False, 'WAN-out ip    0.0.0.255 any 255.255.255.256 established', net4obj)
        # エラー系 : src-port Mnemonic
        self.ut_ismatch_acl(False, 'WAN-out tcp   10.246.0.0 hoge 192.168.80.0 any', net4obj)
        # エラー系 : dst-port Mnemonic
        self.ut_ismatch_acl(False, 'WAN-out tcp   10.246.0.0 any 192.168.80.0 hoge', net4obj)
        pass


if __name__ == '__main__':
    basename = os.path.basename(sys.argv[0])
    dirname = os.path.dirname(sys.argv[0])
    #   CTextFsm.Ntc_Dir = '/usr/lib/python2.7/site-packages/ntc_templates-1.1.0-py2.7.egg/ntc_templates/templates/'
    CTextFsm.Ntc_Dir = dirname + '/ntc_templates/'

    ctextfsm = test()
    #    ctextfsm.put_testacl()
    #    exit(0)
    ctextfsm.ipnet_strict = True
    confobj = ctextfsm.set_acl('./CTextFsm_acl.test')
    net4obj = ctextfsm.parsed2acl(confobj)
    # pprint.pprint(confobj)
    # pprint.pprint(net4obj)
    ctextfsm.DbgLevel[1] = 0
    ctextfsm.DbgLevel[2] = 0

    ctextfsm.ut_acl1()
    ctextfsm.ut_acl2()
