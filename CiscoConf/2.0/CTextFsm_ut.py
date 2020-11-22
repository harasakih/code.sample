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

    def __init__(self):
        CTextFsm.__init__(self)
        pass

    def ut_ipwild2ip(self):
        print 'expect:10.20.30.0/24', MyIpAddress.ipwild2ip('10.20.30.40', '0.0.0.255')
        print 'expect:10.20.0.0/16',  MyIpAddress.ipwild2ip('10.20.30.40', '0.0.255.255')

    def ut_norm_ipmask(self):
        print 'expect:0.0.0.0/255.255.255.255', self.norm_ipmask('any')
        print 'expect:10.20.30.40/0.0.0.0',     self.norm_ipmask('host 10.20.30.40')
        print 'expect:11.22.33.44/0.0.0.0',     self.norm_ipmask('11.22.33.44')
        print 'expect:10.20.30.40/0.0.127.255', self.norm_ipmask('10.20.30.40 0.0.127.255')


    def _ut_is_ipinnet(self):
        self.ut_is_ipinnet(True, '1.2.3.4', '1.2.3.4/32')
        self.ut_is_ipinnet(True, '10.20.30.40', '10.20.30.0/24')
        self.ut_is_ipinnet(False, '1.2.3.4', '10.20.30.0/24')
        
    def _ut_is_ipinnetw(self):
        self.ut_is_ipinnet(True, '1.2.3.4', '1.2.3.4/0.0.0.0')
        self.ut_is_ipinnet(True, '10.20.30.40', '10.20.30.0/0.0.0.255')
        self.ut_is_ipinnet(False, '1.2.3.4', '10.20.30.0/0.0.0.255')

    def _ut_is_port(self):
        print 'ut_port case1:value'
        self.ut_is_port(True, '80', 'eq', '80')
        self.ut_is_port(False, '80', 'eq', '81')
        self.ut_is_port(True, '80', 'range', '80 81')
        self.ut_is_port(True, '81', 'range', '80 81')
        self.ut_is_port(False, '82', 'range', '80 81')
        print 'ut_port case2:any to value'
        self.ut_is_port(False, 'any', 'range', '80 81')
        self.ut_is_port(False, '', 'range', '')
        print 'ut_port case3:value to any'
        self.ut_is_port(True, '80', 'any', '')
        self.ut_is_port(True, '80', '', '80 81')
        print 'ut_port case4:any to any'
        self.ut_is_port(True, '', 'any', '')
        self.ut_is_port(True, '', '', '80 81')
        print 'ut_port case5:established'
        self.ut_is_port(True, 'established', 'established', '')
        self.ut_is_port(True, 'established', '', '')
        self.ut_is_port(False, 'established', 'range', '80 81')
        self.ut_is_port(False, '80', 'established', '')
        pass

    def ut_aclstd(self):
        print "ut_aclstd"
        x = ('_dmy', '60', 'permit', '', '10.246.0.0/0.0.127.255', '', '', '172.20.1.0/24', '', '','log')
        self.ut_ismatch_aclstd(True,  'WAN-out tcp 10.246.0.100 any 172.20.1.0 80', Aclobj._make(x))
        x = ('_dmy', '60', 'permit', '', '10.246.0.0/0.0.0.255', '', '', '172.20.1.0/0.0.0.255', '', '', 'precendence internet')
        self.ut_ismatch_aclstd(False, 'WAN-out tcp 20.246.0.100 any 172.20.1.0 80', Aclobj._make(x))
        pass

    def ut_aclext(self):
        print "ut_aclext"
        self.ut_ismatch_aclext(True,'WAN-out tcp 10.246.0.1 any 172.20.1.0 80',
            Aclobj('_dmy', '60', 'permit', 'tcp', '10.246.0.0/0.0.127.255', '', '', '172.20.1.0/0.0.0.255', '', '',''))
        self.ut_ismatch_aclext(False, 'WAN-out tcp 10.246.0.1 any 172.20.1.0 80',
            Aclobj('_dmy', '60', 'permit', 'tcp', '10.246.0.0/0.0.127.255', '', '', '172.20.1.0/0.0.0.255', 'range','81 82', 'log-in'))
        pass


if __name__ == '__main__':
    basename = os.path.basename(sys.argv[0])
    dirname = os.path.dirname(sys.argv[0])

    c = test()
    c.ut_ipwild2ip()
    c.ut_norm_ipmask()
    c._ut_is_ipinnet()
    c._ut_is_ipinnetw()
    c._ut_is_port()
    c.ut_aclstd()
    c.ut_aclext()