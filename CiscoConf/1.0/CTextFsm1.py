#!/usr/bin/python
# -*- coding: utf-8 -*-
##!/usr/local/bin/python
##!/usr/bin/python
""" Cisco Configをparseするモジュール

ntc_templatesを用いて、解析する。解析可能なテンプレートは、dic:Templ_fnameに格納している。
参考(https://github.com/networktocode/ntc-templates/tree/master/templates)

:Modified 2019/07/24:
  = ntc_templateで、acl_extでip-protocolの時、srcのwildcardは、srcrangeに設定されるため。parse_aclでsrcに設定
  = wild2masklenのバグ修正。オクテット単位にビット長を算出し合算に変更。第４オクテット以外が非ゼロ時に不具合あったため。
  = self.ipnet_strictを追加。is_ipinnetでIPｖ４Networkオブジェクトを作成するときに、ホストビットオンを許容するためFalseを設定
:Modified 2019/07/27:
  = ip-protocolに、eigrp,esp,igmpmpim,ahpを追加
  = tcp&udp-portに、isakmp(500)を追加
:Modified 2019/07/20:
  = sh cdp nei の解析を追加。Split_Header,Templ_fname,Csv_Header
"""

import re
import sys
import os
import pprint
import textfsm
import ipaddress
import math
import inspect  # inspect.stack()[1][3]
from collections import namedtuple
from enum import IntEnum
from enum import Enum

__title__ = 'CTextFsm'
__version__ = '0.0.5'
__date__ = '2019/07/24'
__author__ = 'Hiroshi HARASAKI<mail@example.com>'
__license__ = ''
__copyright__ = 'Copyright (c) 2019 Hiroshi HARASAKI'
__status__ = 'developping'


class MyIpAddress(object):
    def __init__(self):
        pass

    @staticmethod
    def errprint(pgm, caller, msg):
        print >> sys.stderr, '!ERR![' + pgm + ' called by ' + caller + ']', msg
        sys.stdout.flush()
        sys.stderr.flush()

    # noinspection PyUnboundLocalVariable
    @staticmethod
    def ipwild2ip(ip, wild):
        ## DBG-INF
        caller = inspect.stack()[1][3]
        pgm = inspect.getframeinfo(inspect.currentframe())[2]
        ## 例外any
        if ip == '0.0.0.0' and wild == '255.255.255.255':
            net = '0.0.0.0/0'
            return net
        ## 例外host
        if wild == '0.0.0.0':
            net = ip + '/32'
            return net
        ## 
        netaddr = ip + '/' + wild
        try:
            net = ipaddress.IPv4Network(unicode(netaddr, 'utf-8'), strict=False)
        except (ipaddress.NetmaskValueError, ipaddress.AddressValueError, ValueError) as e:
            MyIpAddress.errprint(pgm, caller, 'exception catch:' + str(e))
            MyIpAddress.errprint(pgm, caller, 'invalid ip/mask:' + netaddr)
            net = ip + '/-1'
        finally:
            return str(net)

    @staticmethod
    def is_ipinnet(sip, snet, isstrict=False):
        """ ip-addressがnetwork-addressに含まれるか否かを検査

        :param  sip  :str, like '192.168.0.1'
        :param  snet :str, like '192.168.0.0/24'
        :param  isstrict:bool, IPv4Network-strict
        :return bool :

        :modify 20190724:
          net = ipaddress.IPv4Network()に、strict=Falseを追加。ホストビットオンを無視する。
        """
        ## DBG-INF
        caller = inspect.stack()[1][3]
        pgm = inspect.getframeinfo(inspect.currentframe())[2]
        ##
        try:
            ip = ipaddress.IPv4Address(unicode(sip, 'utf-8'))
        except (ipaddress.AddressValueError, ValueError) as e:
            MyIpAddress.errprint(pgm, caller, 'exception catch:' + str(e))
            return False

        try:
            net = ipaddress.IPv4Network(unicode(snet, 'utf-8'), strict=isstrict)
        except (ipaddress.NetmaskValueError, ipaddress.AddressValueError, ValueError) as e:
            MyIpAddress.errprint(pgm, caller, 'exception catch:' + str(e))
            return False

        if ip in net:
            return True
        else:
            return False
        pass

    @staticmethod
    def is_ipaddress(sip):
        """ ipaddressが有効化否か
        :param sip : str, w.x.y.z
        :return :bool
        """
        ## DBG-INF
        caller = inspect.stack()[1][3]
        pgm = inspect.getframeinfo(inspect.currentframe())[2]
        ##
        if sip == 'any':
            return True
        ##
        try:
            ip = ipaddress.IPv4Address(unicode(sip, 'utf-8'))
        except (ipaddress.AddressValueError, ValueError) as e:
            MyIpAddress.errprint(pgm, caller, 'exception catch:' + str(e))
            return False
        else:
            return True

    @staticmethod
    def is_digit(num):
        """ Numericチェック。[0-9]+

        :param num:str, チェック対象
        :return :bool
        """
        m = re.match(r'^[0-9]+$', num)
        if m is None:
            return False
        else:
            return True


class CiscoShrun(object):
    """docstring for """

    def __init__(self):
        pass

    @staticmethod
    def parse_shrunint_block(fname):
        """ fnameのinterfae情報をブロック単位で抽出する
        :paran fname: str, sh runのファイル名
        return :list, [
          [interface1, [line1,line2,,]],
          [interface2, [line1,line2,,]],
        ]
        """
        rtn = []
        with open(fname, mode='rt') as logf:
            is_interface = False
            for line in logf:
                """ lineは改行コードを含むので改行コードを削除する """
                line = line.rstrip('\n')
                if re.match(r'^interface', line) and is_interface is False:
                    interface, description, ipaddress, ipmask, shutdown, aclg, acldir = '', '', '', '', '', '', ''
                    x = line.split()
                    interface = x[1]
                    is_interface = True
                    block = []
                if is_interface is True:
                    block.append(line)
                ##
                if re.match(r'^!$', line) and is_interface is True:
                    rtn.append([interface, block])
                    is_interface = False
        return rtn
        pass

    @staticmethod
    def grep_shrunint_block(regex, parsedblock):
        """ parse_shrunint_blockの返却値を、regxでgrepする
        """
        rtn = []
        for x in parsedblock:
            [interface, block] = x
            is_found = False
            for line in block:
                m = re.search(r'%s' % regex, line)  # 先頭に限らず検索
                if m is not None:
                    is_found = True
                    break
            else:
                """ not found """
                is_found = False
                pass
            ## loop-out one-interface
            if is_found is True:
                rtn.append([interface, block])
        else:
            ## all-interface is inspected
            return rtn

    @staticmethod
    def grepparse_shrunint(regex, fname):
        parsedblock = CiscoShrun.parse_shrunint_block(fname)
        grep = CiscoShrun.grep_shrunint_block(regex, parsedblock)
        return grep

    @staticmethod
    def parse_shrunint(fname):
        rtn = []
        with open(fname, mode='rt') as logf:
            is_interface = False
            for line in logf:
                """ lineは改行コードを含むので改行コードを削除する """
                line = line.rstrip('\n')
                if re.match(r'^interface', line) and is_interface is False:
                    interface, description, ipaddress, ipmask, shutdown, aclg, acldir = '', '', '', '', '', '', ''
                    x = line.split()
                    interface = x[1]
                    is_interface = True
                if re.match(r'\s+description', line):
                    x = line.split()
                    description = ' '.join(x[1:])
                if re.match(r'^\s+ip\s+address', line):
                    x = line.split()
                    ipaddress = x[2] if len(x) > 2 else ''
                    ipmask = x[3] if len(x) > 3 else ''
                if re.match(r'^\s+no\s+ip\s+address', line):
                    ipaddress = 'no ip address'
                    ipmask = ''
                if re.match(r'^\s+shutdown', line):
                    shutdown = 'shutdown'
                if re.match(r'^\s+ip\s+access-group', line):
                    x = line.split()
                    aclg = x[2]
                    acldir = x[3]
                ##
                if re.match(r'^!$', line) and is_interface is True:
                    if ipaddress is not '' and ipmask is not '' and \
                            ipaddress is not 'no ip address':
                        _addr = ipaddress + '/' + ipmask
                        ipaddress = MyIpAddress.ipwild2ip(ipaddress, ipmask)
                        ipmask = _addr
                    rtn.append([interface, description, ipaddress, ipmask, shutdown, aclg, acldir])
                    is_interface = False
        return rtn


class eRacl(IntEnum):
    """ access-list(racl)の要素番号を持つ、列挙型
    """
    AclName = 0
    AclType = 1
    Comment = 2
    Action = 3
    Protocol = 4
    SrcHost = 5
    SrcAny = 6
    SrcNetwork = 7
    SrcWildCard = 8
    SrcPortMatch = 9
    SrcPort = 10
    SrcPortRangeStart = 11
    SrcPortRangeEnd = 12
    DstHost = 13
    DstAny = 14
    DstNetwork = 15
    DstWildCard = 16
    DstPortMatch = 17
    DstPort = 18
    DstPortRangeStart = 19
    DstPortRAngeEnd = 20
    Log = 21
    Time = 22


class eAcl(IntEnum):
    """ access-list(acl)をテンプレートで解析したOBJの、要素番号を持つ列挙型
    """
    Name = 0
    Sn = 1
    Action = 2
    Protocol = 3
    Source = 4
    Port = 5
    Range = 6
    Dest = 7
    Mod = 8


class eInt(IntEnum):
    """ sh intをテンプレートで解析したOBJの、要素番号を持つ列挙型
    """
    interface = 0  # interface
    link_status = 1  # link_status(administratively down/down/up)
    protocol_status = 2  # down .../up ...
    hardware_type = 3
    address = 4  # mac-address
    bia = 5
    description = 6  # comment, contains SPACE
    ip_address = 7  # ip_address
    mtu = 8
    duplex = 9
    speed = 10
    bandwidth = 11
    delay = 12
    encapsulation = 13
    last_input = 14
    last_output = 15
    last_output_hang = 16
    queue_strategy = 17
    input_rate = 18
    output_rate = 19
    input_packets = 20
    output_packets = 21
    input_errors = 22
    output_errors = 23


Aclobj = namedtuple('Aclobj', (
    'name', 'no', 'action', 'protocol', 'src', 'srcport', 'srcrange', 'dst', 'dstport', 'dstrange', 'other'))
TestPattern = namedtuple('TestPattern', ('name', 'protocol', 'src', 'srcport', 'dst', 'dstport'))


# noinspection PyProtectedMember,PyProtectedMember,PyProtectedMember,PyProtectedMember,PyProtectedMember,PyProtectedMember,PyProtectedMember,PyProtectedMember,PyRedeclaration
class CTextFsm(object):
    """ Cisco ConfをParseするクラス
    """
    ## namedtuple
    ## ciscoのcommand-logを分割するヘッダ
    Split_Header = [
        [r'^.+\#[ ]*sh[ ]+run[ ]*$', '.shrun'],  # sh run
        [r'^.+\#[ ]*sh[ ]+ip[ ]+access\-list[ ]*$', '.shipacl'],  # sh ip access-list
        [r'^.+\#[ ]*sh[ ]+int[ ]+status[ ]*$', '.shintstatus'],  # sh int status
        [r'^.+\#[ ]*sh[ ]+vlan[ ]*$', '.shvlan'],  # sh vlan
        [r'^.+\#[ ]*sh[ ]+ip[ ]+int[ ]+brief[ ]*$', '.shipintbrief'],  # sh ip int brief
        [r'^.+\#[ ]*sh[ ]+ip[ ]+rou[ ]*$', '.shiprou'],  # sh ip rou
        [r'^.+\#[ ]*sh[ ]+ip[ ]+eigrp[ ]+top[ ]+all\-links[ ]*$', '.shipeigrptop'],  # sh eigrp top all-links
        [r'^.+\#[ ]*sh[ ]+ip[ ]+eigrp[ ]+nei[ ]*$', '.shipeigrpnei'],  # sh eigrp nei
        [r'^.+\#[ ]*sh[ ]+int[ ]*$', '.shint'],  # sh int
        [r'^.+\#[ ]*sh[ ]+proc[ ]+cpu[ ]*$', '.shproccpu'],  # sh proc cpu
        [r'^.+\#[ ]*sh[ ]+cdp[ ]+nei[ ]*$', '.shcdpnei']  # sh cdp nei

    ]
    ## ntc-templatesのディレクトリ
    Ntc_Dir = '/usr/lib/python2.7/site-packages/ntc_templates-1.1.0-py2.7.egg/ntc_templates/templates/'
    Ntc_Dir = '/usr/local/lib/python2.7/site-packages/ntc_templates/templates/'
    ## PORT mnemonic
    Port_mnemonic = {
        'ftp-data': 20,
        'ftp': 21,
        'ssh': 22,
        'telnet': 23,
        'smtp': 25,
        'domain': 53,
        'bootps': 67,
        'bootpc': 68,
        'tftp': 69,
        'www': 80,
        'ntp': 123,
        'netbios-ns': 137,
        'netbios-dgm': 138,
        'netbios_ssn': 139,  # 未確認
        'snmp': 161,
        'snmptrap': 162,
        'bgp': 179,
        'https': 443,
        'isakmp': 500,
        'lpd': 515
    }
    ## ntc_templatesのファイル名
    Templ_fname = {
        'racl': 'cisco_ios_show_running-config_partition_access-list.template',
        'acl': 'cisco_ios_show_access-list.template',
        'int': 'cisco_ios_show_interfaces.template',
        'intstatus': 'cisco_ios_show_interfaces_status.template',
        'eigrptop': 'cisco_ios_show_ip_eigrp_topology.template',
        'eigrpnei': 'cisco_ios_show_ip_eigrp_neighbors.template',
        'vlan': 'cisco_ios_show_vlan.template',
        'iprou': 'cisco_ios_show_ip_route.template',
        'rnat': 'cisco_ios_show_running-config_nat.template',
        'cdpnei': 'cisco_ios_show_cdp_neighbors.template'
    }
    ## テンプレートで解析したリストをcsv出力用するヘッダタイトル
    Csv_Header = {
        'racl': ['ACLNAME', 'ACLTYPE', 'COMMENT', 'ACTION', 'PROTOCOL', 'SHOST', 'SANY', 'SNETWORK', 'SWILDCARD',
                 'SPORTMATCH', 'SPORT', 'SPORTRANGESTART', 'SPORTRANGEEND', 'DHOST', 'DANY', 'DNETWORK', 'DWILDCARD',
                 'DPORTMATCH', 'DPORT', 'DPORTRANGESTART', 'DPORTRANGEEND'],
        'acl': ['NAME', 'SN', 'ACTION', 'PROTOCOL', 'SOURCE', 'PORT', 'RANGE', 'DESTINATION', 'MODIFIER'],
        'int': ['INTERFACE', 'LINK_STATUS', 'PROTOCOL_STATUS', 'HARDWARE_TYPE', 'ADDRESS', 'BIA', 'DESCRIPTION',
                'IP_ADDRESS', 'MTU', 'DUPLEX', 'SPEED', 'BANDWIDTH', 'DELAY', 'ENCAPSULATION', 'LAST_INPUT',
                'LAST_OUTPUT', 'LAST_OUTPUT_HANG', 'QUEUE_STRATEGY', 'INPUT_RATE', 'OUTPUT_RATE', 'INPUT_PACKETS',
                'OUTPUT_PACKETS', 'INPUT_ERRORS', 'OUTPUT_ERRORS'],
        'intstatus': ['PORT', 'NAME', 'STATUS', 'VLAN', 'DUPLEX', 'SPEED', 'TYPE'],
        'eigrptop': ['AS', 'ROUTER_ID', 'CODE', 'ROUTE', 'MASK', 'SUCCESSORS', 'FD', 'TAG', 'SERNO', 'FD_AD', 'IF'],
        'eigrpnei': ['AS', 'ADDRESS', 'INTERFACE', 'HOLD', 'UPTIME', 'SRTT', 'RTO', 'Q_CNT', 'SEQ_NUM'],
        'vlan': ['VLAN_ID', 'NAME', 'STATUS', 'INTERFACES'],
        'iprou': ['PROTCOL', 'TYPE', 'NETWORK', 'MASK', 'AD', 'METRIC', 'NEXT-IP', 'NEXT-IF', 'UPTIME'],
        'net4': ['NAME', 'SN', 'ACTION', 'PROTOCOL', 'SRC', 'SPORT', 'SRANGE', 'DST', 'DPORT', 'DRANGE', 'OTHER'],
        'int2': ['INTERFACE', 'LINK_STATUS', 'PROTOCOL_STATUS', 'ADDRESS', 'DESCRIPTION', 'IP_ADDRESS'],
        'rnat': ['INOUT_POOL', 'PROTOCOL', 'SOURCE', 'FROM_IP', 'FROM_PORT', 'TO_IP', 'TO_PORT', 'MASK', 'ACL',
                 'POOL_NAME', 'POOL_SIP', 'POOL_EIP', 'POOL_MASK', 'RMAP_NAME'],
        'cdpnei': ['NEIGHBOR', 'LOCAL_INTERFACE', 'CAPABILITY', 'PLATFORM', 'NEIGHBOR_INTERFACE'],
        'rint': ['INTERFACE', 'DESC', 'IPADR', 'IPMASK', 'SHUT', 'ACLG', 'ACLDIR']
    }
    ## Aclのactionに許可される語句
    IfAction = {
        'permit': 0,
        'deny': 1
    }
    ## IP-Protocols、IPの上位プロトコルなので、IPのフィルタをチェックする
    IfIpProtocol = {
        'ip': 0,
        'icmp': 1,
        'igmp': 2,
        'tcp': 6,
        'udp': 17,
        'esp': 50,
        'ahp': 51,
        'eigrp': 88,
        'ospf': 89,
        'pim': 103
    }

    ## DBGログの出力レベル
    DbgLevel = [
        0,  # LV0   何も出力しない。
        0,  # LV1    ismatch_aclで、テストパターン毎に情報を出力
        0,  # LV2    ismatch_aclで、access-list毎に情報を出力
        0,  # LV3    複数ファイル検索で、検索対象ファイル名を出力
        0  # LV4
    ]

    def __init__(self):
        """ Constructor
        """
        ## CiscoConfigのファイル名
        self.confname = None
        ## テンプレートファイル名
        self.templfname = None
        ## dic:aclobjのキー一覧
        self.aclnames = []
        ## 処理中のテストパターン
        self.testpattern = ''
        ## 処理中のacl-statement
        self.acl_statement = []
        ## 処理中のparsed-statement
        self.parse_statement = []
        ## is_ipinnetで、ホストビットオンをチェックするか否か
        self.ipnet_strict = False

    def _ismatch_aclstd(self, testptn, a_aclobj):
        """ Standard ACL をチェックする（送信元IPのみ）

        :param testptn  :str, テスト対象の通信パターン[ACL名, プロトコル, 送信元IP, 送信元PORT, 送信先IP, 送信先PORT]
        :param a_aclobj :Aclobj, accsecc-listの一つ分、IPアドレスはwilcard-maskをbit長に変換後
        :return : [ismatch, [is_action, is_prot, is_src]]
        :modify: 2019/08/04,性能対策
          namedtupleは関数間IFのみで使用、関数内は変数に代入して使用する
        """
        ## DBG-INF
        caller = inspect.stack()[1][3]
        pgm = inspect.getframeinfo(inspect.currentframe())[2]
        ##
        if type(testptn) is str:
            test = TestPattern._make(testptn.split())
        else:
            test = testptn
        ## 20190804:性能対策のため、変数に代入し使用する
        test_name = test.name
        test_protocol = test.protocol
        test_src = test.src
        test_srcport = test.srcport
        test_dst = test.dst
        test_dstport = test.dstport
        ##
        a_aclobj_name = a_aclobj.name
        a_aclobj_no = a_aclobj.no
        a_aclobj_action = a_aclobj.action
        a_aclobj_protocol = a_aclobj.protocol
        a_aclobj_src = a_aclobj.src
        a_aclobj_srcport = a_aclobj.srcport
        a_aclobj_srcrange = a_aclobj.srcrange
        a_aclobj_dst = a_aclobj.dst
        a_aclobj_dstport = a_aclobj.dstport
        a_aclobj_dstrange = a_aclobj.dstrange
        a_aclobj_other = a_aclobj.other
        ## テストパターンの正規化
        if test_src == 'any':
            test_src = '0.0.0.0'
        if test_dst == 'any':
            test_dst = '0.0.0.0'
        ## チェック
        if a_aclobj_protocol != '':
            self.errprint(pgm, caller, 'acl-PROTOCOL not null')
            is_prot = False
        else:
            is_prot = True

        is_action = True if a_aclobj_action in CTextFsm.IfAction else False
        is_src = MyIpAddress.is_ipinnet(test_src, a_aclobj_src, self.ipnet_strict)

        if is_action == True and is_prot == True and is_src == True:
            return [True, [is_action, is_prot, is_src]]
        else:
            return [False, [is_action, is_prot, is_src]]

    def _ismatch_aclext(self, testptn, a_aclobj):
        """ Extended ACL をチェクする（PROTCOL,IP,PORT）

        :param testptn  :str, テスト対象の通信パターン[ACL名, プロトコル, 送信元IP, 送信元PORT, 送信先IP, 送信先PORT]
        :param a_aclobj :Aclobj, accsecc-listの一つ分、IPアドレスはwilcard-maskをbit長に変換後
        :return : [ismatch, [is_action, is_prot, is_src, is_dst, is_sport, is_dport]]
        :modify: 2019/08/04,性能対策
          namedtupleは関数間IFのみで使用、関数内は変数に代入して使用する
        """
        ##
        caller = inspect.stack()[1][3]
        pgm = inspect.getframeinfo(inspect.currentframe())[2]
        ##
        if type(testptn) is str:
            test = TestPattern._make(testptn.split())
        else:
            test = testptn
        ## 20190804:性能対策のため、変数に代入し使用する
        test_name = test.name
        test_protocol = test.protocol
        test_src = test.src
        test_srcport = test.srcport
        test_dst = test.dst
        test_dstport = test.dstport
        ##
        a_aclobj_name = a_aclobj.name
        a_aclobj_no = a_aclobj.no
        a_aclobj_action = a_aclobj.action
        a_aclobj_protocol = a_aclobj.protocol
        a_aclobj_src = a_aclobj.src
        a_aclobj_srcport = a_aclobj.srcport
        a_aclobj_srcrange = a_aclobj.srcrange
        a_aclobj_dst = a_aclobj.dst
        a_aclobj_dstport = a_aclobj.dstport
        a_aclobj_dstrange = a_aclobj.dstrange
        a_aclobj_other = a_aclobj.other
        ## テストパターンの正規化
        if test_src == 'any':
            test_src = '0.0.0.0'
        if test_dst == 'any':
            test_dst = '0.0.0.0'
        ## チェック
        is_action = True if a_aclobj_action in CTextFsm.IfAction else False
        if a_aclobj_protocol != '':
            is_prot = self.is_prot(test_protocol, a_aclobj_protocol)
        else:
            self.errprint(pgm, caller, 'acl-PROTOCOL is null')
            is_prot = False

        if a_aclobj_src != '':
            is_src = MyIpAddress.is_ipinnet(test_src, a_aclobj_src, self.ipnet_strict)
        else:
            self.errprint(pgm, caller, 'acl-SRC-IP is null')
            is_src = False
        pass
        if a_aclobj_dst != '':
            is_dst = MyIpAddress.is_ipinnet(test_dst, a_aclobj_dst, self.ipnet_strict)
        else:
            self.errprint(pgm, caller, 'acl-DST-IP is null')
            is_dst = False
        pass

        """ プロトコルをPORTCKの組合わせ
            テスト通信がIPの上位プロトコルで、フィルタがIPの場合は、このIPフィルタをチェックし、PORTチェックはしない。
            テスト通信＝フィルタ＝(tcp,udp)の場合は、PORTをチェックする。
        """
        if (test_protocol == 'tcp' and a_aclobj_protocol == 'tcp') or \
                (test_protocol == 'udp' and a_aclobj_protocol == 'udp'):
            pass  # PORTをチェック
        else:
            if (is_action == True and is_prot == True and
                    is_src == True and is_dst == True):
                return [True, [is_action, is_prot, is_src, is_dst]]
            else:
                return [False, [is_action, is_prot, is_src, is_dst]]
        pass
        ## PORTチェックする、プロトコルの場合。拡張ACL
        if test_protocol == 'tcp' or test_protocol == 'udp':
            if test_srcport != '':
                is_sport = self.is_port(test_srcport, a_aclobj_srcport, a_aclobj_srcrange)
            else:
                self.errprint(pgm, caller, 'test-SRC-PORT is null')
                self.errprint(pgm, caller, test)
                self.errprint(pgm, caller, a_aclobj)
                is_sport = False
            ###
            if test_dstport != '':
                is_dport = self.is_port(test_dstport, a_aclobj_dstport, a_aclobj_dstrange)
            else:
                self.errprint(pgm, caller, 'test-DST-PORT is null')
                self.errprint(pgm, caller, test)
                self.errprint(pgm, caller, a_aclobj)
                is_dport = False
            ### return to caller
            if is_action == True and is_prot == True and \
                    is_src == True and is_dst == True and \
                    is_sport == True and is_dport == True:
                return [True, [is_action, is_prot, is_src, is_dst, is_sport, is_dport]]
            else:
                return [False, [is_action, is_prot, is_src, is_dst, is_sport, is_dport]]
            ###
        else:
            self.errprint(pgm, caller, 'unexpected Error')
            self.errprint(pgm, caller, test)
            self.errprint(pgm, caller, a_aclobj)
            return [False, []]
        pass

    def ismatch_acl(self, testptn, aclobjs, tfonly=False):
        """ ACLフィルタを格納した辞書を受け取り、テストする

        :param testptn :str, テスト対象の通信パターン.[ACL名, プロトコル, 送信元IP, 送信元PORT, 送信先IP, 送信先PORT]
        :param aclobjs :dic Aclobj, Coisco Configをパースした辞書、parsed2aclで作成
               aclobjs[ acl名 ] = [ (Aclobj),(Aclobj),() ]
        :param tfonly  :bool, True/False（access-listに合致したか否か）のみ返却か、合致したaclも返却するか
        :return : list,
          [
          bool   : is_match
          str    : acl名 or ERR/DENY
          str    : aclのaction(permit/deny/'')
          Aclobj : 合致したacl-statement
          ]
        :modify: 2019/08/04,性能対策
          namedtupleは関数間IFのみで使用、関数内は変数に代入して使用する
        """
        ## DBG-INF
        caller = inspect.stack()[1][3]
        pgm = inspect.getframeinfo(inspect.currentframe())[2]
        ##
        ## testptnの項目数チェック
        if type(testptn) is str:
            tmp = testptn.split()
            if len(tmp) == 6:
                test = TestPattern._make(tmp)
            else:
                self.errprint(pgm, caller, 'num of testptn not 6:' + testptn)
                self.errprint(pgm, caller, 'testptn like "acl-name prot src-ip src-port dst-ip dst-port"')
                return [False, '', 'ERR', []]
        else:
            test = testptn
        pass

        ## 20190804:性能対策のため、変数に代入して使用
        test_name = test.name
        test_protocol = test.protocol
        test_src = test.src
        test_srcport = test.srcport
        test_dst = test.dst
        test_dstport = test.dstport

        ## 単項目チェック(NAME)
        if test_name in aclobjs:
            aclobj = aclobjs[test_name]
        else:
            self.errprint(pgm, caller, 'aclname not Known:' + test_name)
            return [False, test_name, 'ERR', []]

        self.testpattern = testptn
        self.dbgprint(1, pgm, (testptn + '->'))
        ##
        ## testptnの単項目チェック(PROTOCOL)
        ##
        if not test_protocol in CTextFsm.IfIpProtocol:
            self.errprint(pgm, caller, 'protocol not Known:' + test_protocol)
            return [False, test_name, 'ERR', []]
        ##
        ## testptnの単項目チェック(SRC-IP,DST-IP)
        ##
        if MyIpAddress.is_ipaddress(test_src) == False or MyIpAddress.is_ipaddress(test_dst) == False:
            return [False, test_name, 'ERR', []]
        ##
        ## testptnの単項目チェック(SRC-PORT)
        ##
        ### any, established, \d+
        if (not test_srcport in ['any', 'established'] and
                not re.match(r'^\d+$', test_srcport)):
            self.errprint(pgm, caller, 'src-port is not numeric:' + test_srcport)
            return [False, test_name, 'ERR', []]
        ##
        ## testptnの単項目チェック(DST-PORT)
        ##
        ### any, established, \d+
        if (not test_dstport in ['any', 'established'] and
                not re.match(r'^\d+$', test_dstport)):
            self.errprint(pgm, caller, 'dst-port is not numeric:' + test_dstport)
            return [False, test_name, 'ERR', []]
        ##
        ## ACL分を繰り返し検査
        ##
        for x in aclobj:
            a_aclobj = x
            self.acl_statement = str(a_aclobj)
            self.dbgprint(2, pgm, a_aclobj)
            ##
            if a_aclobj.protocol == '':
                [tf, is_flgs] = self._ismatch_aclstd(test, a_aclobj)
            else:
                [tf, is_flgs] = self._ismatch_aclext(test, a_aclobj)
            pass
            self.dbgprint(2, pgm, is_flgs)
            if tf:
                rtn = tf if tfonly == True else [tf, test_name, a_aclobj.action, a_aclobj]
                return rtn
            pass  # 次を検索
        else:
            return [False, test_name, 'DENY', []]
        pass

    def read_conf(self, fname):
        """ ファイルを読込み、リストで返却する
        :param str fname : ファイル名
        :return list     : .read()で返却されたリスト。ファイル全体を返却。
        """
        ## open cisco-cfg(showrun)
        self.confname = fname
        with open(fname, mode='r') as cfg:
            confobj = cfg.read()
        return confobj

    def parse_racl(self, confobj, tdir=None):
        """ Cisco Confをntc_templatesでパースし、ntc_templatesの形式で返却。
        confは、[show runのaccess-list部分]

        :param confobj :list, 'show run'のaccess-list部分を読みこんだリスト
        :param tdir    :str, テンプレートのディレクトリ
        :return :list, テンプレート形式のリスト
        """
        ## テンプレートは[racl]
        templ = 'racl'
        lst = self.parse(confobj, templ, tdir)
        ## lst 含まれる [ip access-list TYPE ACL_NAME] を除外する
        rtn = []
        for l in lst:
            if l[eRacl.Action] != '':
                rtn.append(l)
        return rtn

    def parse_acl(self, confobj, tdir=None):
        """ Cisco Confをntc_templatesでパースし、ntc_templatesの形式で返却
        confは、[show ip access-list]

        :param confobj :list, 'show ip access-list'の結果を読みこんだリスト
        :param tdir    :str, テンプレートのディレクトリ
        :return :list, テンプレート形式のリスト
        """
        ## テンプレートは[acl]
        templ = 'acl'
        lst = self.parse(confobj, templ, tdir)
        ## lst 含まれる [ip access-list TYPE ACL_NAME] を除外する
        rtn = []
        for l in lst:
            if l[eAcl.Action] != '':
                ## ntc-templateの補正をする
                ### Modifier の(...)を消去する
                l[eAcl.Mod] = re.sub(r'[ ]*\(.+\)$', '', l[eAcl.Mod])

                ### protocol=ipの時、sourceのwild-maskをRangeからSourceに移動
                if l[eAcl.Protocol] == 'ip' and l[eAcl.Range] != '':
                    l[eAcl.Source] += (' ' + l[eAcl.Range])
                    l[eAcl.Range] = ''
                rtn.append(l)
        return rtn

    def parse(self, confobj, templ=None, tdir=None):
        """ Cisco Confをntc_templatesでパースし、ntc_templatesの形式で返却
        confはtemplにあったもの

        :param  confobj :list, templにあったもの
        :param  templ   :str, テンプレート名
        :param  tdir    :str, テンプレートのディレクトリ
        :return :list, テンプレート形式のリスト。confobjをテンプレートで解析した結果
        """
        if templ is None:
            return None
        if tdir is None:
            templfname = CTextFsm.Ntc_Dir + CTextFsm.Templ_fname[templ]
        else:
            templfname = tdir + CTextFsm.Templ_fname[templ]
        return self._parse(confobj, templfname)

    def _parse(self, confobj, templfname=None):
        """ Cisco Config をntc_tenplatesでパースし、テンプレート形式で返却

        :param  confobj    :list, Cisco-Configを保持するリスト
        :param  templfname :str,  テンプレートファイル名
        :return :list, テンプレート形式のリスト。confobjをテンプレートで解析した結果
        """
        self.templfname = templfname
        with open(templfname) as templ:
            table = textfsm.TextFSM(templ)
            result = table.ParseText(confobj)
        lst = result

        rtn = []
        for l in lst:
            rtn.append(l)
        return rtn

    # noinspection PyUnusedLocal,PyUnusedLocal
    def parsed2acl(self, acls):
        """ parse_acl()を編集する。wildcard-maskを/nnに変換、PORT番号のMnemonicを数値化

        :param  acls  :list, parse_acl()でテンプレート化したリスト
        :return :dic Aclobj,
          dic[ acl-name ] = [ Aclobj, Aclobj, ] を返却する。
          Aclobj = [name, no, action, protocol, src, srcport, srcrange, dst, dstport, dstrange]
        """
        ## DBG-INF
        caller = inspect.stack()[1][3]
        pgm = inspect.getframeinfo(inspect.currentframe())[2]
        ##
        rtn = {}  # return is dictonary
        for acl in acls:
            self.parse_statement = acl
            name = acl[eAcl.Name]
            no = acl[eAcl.Sn]
            action = acl[eAcl.Action]
            protocol = acl[eAcl.Protocol]
            # numberd access-list
            (srcip, srcmask) = self.norm_ipmask(acl[eAcl.Source])
            (dstip, dstmask) = self.norm_ipmask(acl[eAcl.Dest])

            ## src-Port
            if acl[eAcl.Port] in ['', 'eq', 'gt', 'lt', 'neq', 'range']:
                srcport = acl[eAcl.Port]
            else:
                srcport = acl[eAcl.Port]
                self.errprint(pgm, caller, 'unKnown src-port:' + str(srcport))
            pass

            ## src-range
            tmp = acl[eAcl.Range].split()  # numric or mnemonic
            """ (SRC)Rangeのニーモニックを数字に変換
            """
            x = []
            for t in tmp:
                if t in self.Port_mnemonic:
                    x.append(str(self.Port_mnemonic[t]))
                elif MyIpAddress.is_digit(t):
                    x.append(str(t))
                else:
                    self.errprint(pgm, caller, 'unKnown src-mnemonic:' + str(t))
                    self.errprint(pgm, caller, str(self.parse_statement))
                    x.append(str(t))
            srcrange = ' '.join(x)

            ## Modifierを、(dstport, dstrange, other)に分解する
            """
            ptn0 ''
            ptn1 eq/lt/gt/neq  XXX      [precendence *] [log]
            ptn2 range         XXX YYY  [precendence *] [log]
            prn3 established            [precendence *] [log]
            ptn4 [precendence *] [log]
            """
            tmp = acl[eAcl.Mod].split()
            if len(tmp) == 0:
                dstport = ''
                dstrange = ''
                other = ''
            else:
                _port = []
                _other = []
                dstport = ''
                dstrange = ''
                other = ''

                i = 0
                while i < len(tmp):
                    if tmp[i] in ['eq', 'gt', 'lt', 'neq']:
                        dstport = tmp[i]
                        p = tmp[i + 1]
                        if p in self.Port_mnemonic:
                            _port.append(str(self.Port_mnemonic[p]))
                        elif MyIpAddress.is_digit(p):
                            _port.append(str(p))
                        else:
                            self.errprint(pgm, caller, 'unKnown dst1-mnemonic:' + str(p))
                            self.errprint(pgm, caller, str(self.parse_statement))
                            _other.append(p)
                        i += 1
                    elif tmp[i] in ['range']:
                        dstport = tmp[i]
                        """ min_PORT """
                        p = tmp[i + 1]
                        if p in self.Port_mnemonic:
                            _port.append(str(self.Port_mnemonic[p]))
                        elif MyIpAddress.is_digit(p):
                            _port.append(str(p))
                        else:
                            self.errprint(pgm, caller, 'unKnown dst2-mnemonic:' + str(p))
                            self.errprint(pgm, caller, str(self.parse_statement))
                            _other.append(str(p))
                        """ max_PORT """
                        p = tmp[i + 2]
                        if p in self.Port_mnemonic:
                            _port.append(str(self.Port_mnemonic[p]))
                        elif MyIpAddress.is_digit(p):
                            _port.append(str(p))
                        else:
                            self.errprint(pgm, caller, 'unKnown dst3-mnemonic:' + str(p))
                            self.errprint(pgm, caller, str(self.parse_statement))
                            _other.append(str(p))
                        """ increment i by 2 """
                        i += 2
                    elif tmp[i] in ['established']:
                        dstport = tmp[i]
                    else:
                        _other.append(tmp[i])
                    pass
                    i += 1
                pass
                dstrange = ' '.join(_port)
                other = ' '.join(_other)
            pass

            if srcmask != '':
                src = MyIpAddress.ipwild2ip(srcip, srcmask)
            else:
                src = srcip

            #
            if dstmask != '':
                dst = MyIpAddress.ipwild2ip(dstip, dstmask)
            else:
                dst = dstip
            #
            if not name in rtn:
                rtn[name] = []
            ## hiroshi aclobjの返却
            a_aclobj = Aclobj._make([name, no, action, protocol, src, srcport, srcrange, dst, dstport, dstrange, other])
            rtn[name].append(a_aclobj)
        else:
            self.aclnames = rtn.keys()
            return rtn
        pass

    def conf_csvprint(self, confname, templ, isheader=True, headding=None):
        """ Cisco configファイルをCSV出力する

        :param confname :str,  confのファイル名
        :param templ    :str,  template名
        :param isheader :bool, CSVヘッダを出力するか否か
        :param headding :str,  先頭カラムに出力する文字列、confnameを想定。
        """
        ## DBG-INF
        caller = inspect.stack()[1][3]
        pgm = inspect.getframeinfo(inspect.currentframe())[2]
        ##
        header = []
        if templ in self.Templ_fname:
            if templ in self.Csv_Header:
                header = self.Csv_Header[templ]
            else:
                header = None
        else:
            self.errprint(pgm, caller, 'Not Known Template:' + templ)
        pass
        ##
        if headding is None:
            header = None if isheader is not True else header
            csv = self.set_parsed(confname, templ)
            CTextFsm.csvprint(csv, header, headding=None)
        else:
            header.insert(0, 'CONFNAME')
            header = None if isheader is not True else header
            csv = self.set_parsed(confname, templ)
            CTextFsm.csvprint(csv, header, headding)
        pass

    def logsplit(self, fname, otdir='./', otfbase=None):
        """ Ciscoの各種照会コマンドログをコマンド単位で分割する
        fnameを分割し、fname.ext.txtを作成する。extは、Split_Header[]で指定され、ドットを含む。

        :param fname   :str, コマンドログのファイル名
        :param otdir   :str, 出力先ディレクトリ
        :param otfbase :str, 出力ファイルのファイル名ベース（デフォルトは入力ファイル名）
        """
        with open(fname, mode='rt') as logf:
            otfname = ''
            skip = 1
            for line in logf:
                """ lineは改行コードを含むので改行コードを削除する """
                line = line.rstrip('\n')
                for heads in CTextFsm.Split_Header:
                    header = heads[0]
                    ext = heads[1]
                    ## ログファイルが、ヘッダに合致したらファイルを分割出力する。
                    if re.match(header, line) is not None:
                        """
                        if otfbase is None:
                            otfname = fname + ext + '.txt'
                        else:
                            l = len(otdir)
                            if otdir[l-1] == '/':
                                otfname = otdir + otfbase + ext + '.txt'
                            else:
                                otfname = otdir + '/' + otfbase + ext + '.txt'
                            pass
                        pass
                        """
                        ## 20190730 ファイル名の正規化
                        if otfbase is None:
                            fname = os.path.basename(fname)
                            _otfname = fname + ext + '.txt'
                        else:
                            _otfname = otfbase + ext + '.txt'
                        ## 20190730 出力ディレクトリの正規化
                        l = len(otdir)
                        if otdir is None:
                            otfname = _otfname
                        elif otdir[l - 1] == '/':
                            otfname = otdir + _otfname
                        else:
                            otfname = otdir + '/' + _otfname
                        pass

                        self.infprint('logsplit', '', 'open  ' + line + '->' + otfname)
                        otf = open(otfname, mode='w')
                        skip = 2
                        break
                pass
                ## 分割出力中に、コマンドプロンプトが出現したら、ファイルをクローズ
                """
                出力ファイルがオープンされていて
                ヘッダ行ではなく
                空白以外＋＃に合致したら
                """
                if otfname != '' and \
                        re.match(header, line) is None and \
                        re.match(r'^\S+#', line) is not None:
                    self.infprint('logsplit', '', 'close ' + line)
                    otf.close()
                    otfname = ''
                    skip = 1

                if skip == 1 and otfname == '':
                    skip = 1
                elif skip == 2 and otfname != '':
                    skip = 0
                else:
                    skip = 0
                    """ コマンドエラーの対処。エラー出力は空白として出力
                    """
                    m = re.match(r'(^\s+\^\s*$)|(\% Invalid input detected at)|(\% Ambiguous command)', line)
                    if m is None:
                        """ lineは改行コードを含まないので付加 """
                        otf.write(line + '\n')
                pass
            pass
            if otfname != '':
                otf.close()

    def norm_ipmask(self, ipmask):
        """ 'ip-address wildcard-mask'を正規化する

        :param ipmask :str, like 'ip wildmask'
        :return :2*str ip-address, wildcard-mask

        変換概要
          [any]
            ''  -> ''
            any -> 0.0.0.0 255.255.255.255  -> 0.0.0.0/0    
          [host]
            w.x.y.z -> w.x.y.z 0.0.0.0       -> w.x.y.z/32
            host w.x.y..z -> w.x.y.z 0.0.0.0 -> w.x.y.z/32
          [network]
            w.x.y.z, wildcard bits W.X.Y.Z   -> w.x.y.z W.X.Y.Z
            w.x.y.z W.X.Y.Z -> w.x.y.z W.X.Y.Z
          [ERROE]
            ELSE(ERROR) -> None
        """
        ## DBG-INF
        caller = inspect.stack()[1][3]
        pgm = inspect.getframeinfo(inspect.currentframe())[2]
        ##
        if ipmask == '':
            return '', ''

        if ipmask == 'any':
            return '0.0.0.0', '255.255.255.255'

        ## IPアドレスのみ
        if re.match(r'^(\d+\.\d+\.\d+\.\d+)$', ipmask):
            return ipmask, '0.0.0.0'
        ## ip-address と wildmask に分割
        tmp = ipmask.split(' ')
        ## host IP-Address
        if tmp[0] == 'host':
            """ 0802 modify """
            return tmp[1], '0.0.0.0'
        ## IP, wildcard bits mask
        elif tmp[1] == 'wildcard':
            l = len(tmp[0])
            ip = tmp[0][0:l - 1]
            mask = tmp[3]
            return ip, mask
        else:
            m = re.match(r'(^\d+\.\d+\.\d+\.\d+)(\s+)(\d+\.\d+\.\d+\.\d+$)', ipmask)
            if m is not None:
                (ip, dmy, mask) = m.groups()
                return ip, mask
            else:
                self.errprint(pgm, caller, ('invalid ipmask :%s' % ipmask))
                return None, None
        pass

    def is_port(self, port, ope, range):
        """ Cisco access-listのPORT番号が、合致するか否かを検査

        :param  port  :str, テスト対象のPORT番号。数字
        :param  ope   :str, eq/range/gt/lt/established or null
        :param  range :str, テスト対象のPORT範囲。like 'xxx yyy'
        :return :bool
        ----
                port    ope     range       is_port
        CASE1   value   OPE     value       check : rangeをチェック
        CASE2   ANY     OPE     value       False : any > value なのでFalse
        CASE3   value   ANY     --          True  : value < any なのでTrue
        CASE4   ANY     ANY     --          True  : any = any   なのでTrue
        CASE21  EST     EST     --          True  : EST == EST
        CASE22  EST     OPE    value        False : EST != not est
        CASE23  else    EST     --          False : not est != establieshed
        """
        ## DBG-INF
        caller = inspect.stack()[1][3]
        pgm = inspect.getframeinfo(inspect.currentframe())[2]
        ## portの正規化
        port = 'ANY' if (port == '' or port == 'any') else port
        ## opeの正規化
        ope = 'ANY' if (ope == '' or ope == 'any') else ope

        ## CASE4,3
        if ope == 'ANY':
            return True

        ## CASE2例外
        if port == 'established' and ope == 'established':
            return True
        elif port == 'established' and ope in ['eq', 'range', 'gt', 'lt']:
            return False
        elif port != '' and ope == 'established':
            return False

        ## CASE2
        if port == 'ANY':
            return False

        ## CASE1
        try:
            iport = int(port)
        except ValueError as e:
            self.errprint(pgm, caller, e)
            self.errprint(pgm, caller, ('port=%s ope=%s range=%s' % (port, ope, range)))
            return False
        else:
            pass

        ##
        ret = False
        if ope == 'range':
            tmp = range.split(' ')
            irangemin = int(tmp[0])
            irangemax = int(tmp[1])
            if irangemin <= iport <= irangemax:
                return True
            else:
                return False
        ##
        irange = int(range)
        if ope == 'eq':
            ret = True if iport == irange else False
        ##
        elif ope == 'neq':
            ret = True if iport != irange else False
        ##
        elif ope == 'gt':
            ret = True if iport > irange else False
        ##
        elif ope == 'lt':
            ret = True if iport < irange else False
        ##
        return ret

    @staticmethod
    def is_prot(tprot, fprot):
        """ プロトコルが一致するか

        :param  tprot:str, test-port
        :param  fprot:str, acl-port
        :return :bool

        チェック概要
            tprot   fprot       is_prot PORTCK
            ip      ip          ok      no
            eigrp   ip,eigrp    ok      no
            ospf    ip,ospf     ok      no
            tcp     ip          ok      no
            udp     ip          ok      no
            hoge    ip          ng      no
            hoge    hoge        ok      no
        ----
            tcp     tcp         ok      yes
            tcp     !tcp        ng      -
            udp     udp         ok      yes
            udp     !udp        ng      -
        """
        if (fprot == 'ip') and (tprot in CTextFsm.IfIpProtocol):
            return True
        elif tprot == fprot:
            return True
        else:
            return False
        pass

    def set_parsed(self, fname=None, templ=None, tdir=None):
        """ confnameを入力し、templateでパースしたリストを返却
        :param fname :str, confname
        :param templ :str, template-NAME
        :param tdir  :str, templateのディレクトリ
        :return :list, tenpateでparseしたリスト
        """
        return self.parse(self.read_conf(fname), templ, tdir)

    def set_acl(self, fname=None, tdir=None):
        """ confnameを入力し、template(acl)でパースしたリストを返却
        :param  fname :str, confname
        :param  tdir  :str, templateのディレクトリ
        :return :list, 'acl'でparseしたリスト
        """
        return self.parse_acl(self.read_conf(fname), tdir)

    def set_racl(self, fname=None, tdir=None):
        """ confnameを入力し、template(racl)でパースしたリストを返却
        :param  fname :str, confname
        :param  tdir  :str, templateのディレクトリ
        :return list :list, 'racl'でparseしたリスト
        """
        return self.parse_racl(self.read_conf(fname), tdir)

    @staticmethod
    def infprint(pgm, caller, msg, out=None):
        if out is None:
            print >> sys.stderr, '!INF![' + pgm + ' called by ' + caller + ']', msg
            sys.stdout.flush()
            sys.stderr.flush()
        else:
            print >> out, '!INF![' + pgm + ' called by ' + caller + ']', msg
            sys.stdout.flush()
            sys.stderr.flush()

    @staticmethod
    def errprint(pgm, caller, msg):
        print >> sys.stderr, '!ERR![' + pgm + ' called by ' + caller + ']', msg
        sys.stdout.flush()
        sys.stderr.flush()

    def dbgprint(self, level, pgm, msg):
        if self.DbgLevel[level] != 0:
            print >> sys.stderr, '!DBG![' + pgm + ' ' + str(level) + ']', msg
            sys.stdout.flush()
            sys.stderr.flush()

    def ut_is_port(self, expect, port, ope, range):
        """ UT-driver for is_port()

        :param expect :bool, UT結果の期待値
        :param port   :str, port
        :param ope    :str, ope
        :param range  :str, range
        """
        ret = self.is_port(port, ope, range)
        okng = 'UTOK' if ret == expect else 'UTNG'
        print okng, ret, port, ope, range

    def ut_is_ipinnet(self, expect, ip, net):
        """ UT-driver for ut_is_ipinnet

        :param expect :bool, UT結果の期待値
        :param ip     :str, ip-address   'w.x.y.z'
        :param net    :str, network like 'W.X.Y.Z/L'
        """
        try:
            ret = MyIpAddress.is_ipinnet(ip, net, self.ipnet_strict)
        except ValueError as e:
            ret = False
            print e
        okng = 'UTOK' if ret == expect else 'UTNG'
        print okng, ret, ip, net

    def ut_ismatch_acl(self, expect, test, aclobj):
        """ UT-driver for ismatch_acl

        :param expect :bool, UT結果の期待値
        :param test   :str,  test-pattern with aclname。'WAN-out eigrp 10.246.0.1 any 10.246.0.1 any'
        :param aclobj :list,　.parsed2acl(confobj)の返却
        """
        [tf, name, action, matched_aclobj] = self.ismatch_acl(test, aclobj)
        okng = 'UTOK' if tf == expect else 'UTNG'
        print okng, test, '->', name, action, tuple(matched_aclobj)

    def ut_ismatch_acl2(self, expect, aclname, test, aclobj):
        """ UT-driver for ismatch_acl2
        aclname,testを別パラメタ

        :param expect  :bool, UT結果の期待値
        :param aclname :str,  aclobjからaclnameをチェック対象とする。'WAN-out'
        :param test    :str,  test-pattern。'ip    10.246.0.1 any 10.246.0.1 any'
        :param aclobj  :list, .parsed2acl(confobj)の返却
        """
        [tf, name, action, matched_aclobj] = self.ismatch_acl(aclname + ' ' + test, aclobj)
        okng = 'UTOK' if tf == expect else 'UTNG'
        print okng, aclname, test, '->', name, action, tuple(matched_aclobj)

    # noinspection PyUnusedLocal
    def ut_ismatch_aclstd(self, expect, test, a_aclobj):
        """ UT-driver for _ismatch_aclstd

        :param expect   :bool, UT結果の期待値
        :param test     :str, 'WAN-out tcp 10.246.0.100 any 172.20.1.0 80'
        :param a_aclobj :namedtuple Aclobj
          Aclobj('_dmy', '60', 'permit', '', '10.246.0.0/17', '', '', '172.20.1.0/24', '', '', 'log')
        """
        [tf, flgs] = self._ismatch_aclstd(test, a_aclobj)
        okng = 'UTOK' if tf == expect else 'UTNG'
        print okng, test, '->', tuple(a_aclobj)

    # noinspection PyUnusedLocal
    def ut_ismatch_aclext(self, expect, test, a_aclobj):
        """ UT-driver _ismatch_aclext

        :param expect   :bool, UT結果の期待値
        :param test     :str,
        :param a_aclobj :namedobj Aclobj
          Aclobj('_dmy', '60', 'permit', 'tcp', '10.246.0.0/17', '', '', '172.20.1.0/24', '', '', '')
        """
        [tf, flgs] = self._ismatch_aclext(test, a_aclobj)
        okng = 'UTOK' if tf == expect else 'UTNG'
        print okng, test, '->', tuple(a_aclobj)

    # noinspection PyMethodMayBeStatic
    def get_enum(self):
        print eRacl
        print list(eRacl)
        print eAcl
        print list(eAcl)

    @staticmethod
    def csvprint(ll, header=None, headding=None):
        """ リストをTSV出力する。タイトルヘッダ、１カラム目にheaddingを出力可

        :param ll       :list, [[row1,row2,row3,],[row1,row2,row3,],[row1,row2,row3,],]
        :param header   :list, CSVヘッダ
        :param headding :str,
        """
        """ headerの出力
        """
        if header is not None:
            for t in header:
                sys.stdout.write(t + '\t')
            sys.stdout.write('\n')
        """ csvの出力
        """
        for l in ll:
            """ headdingの出力 """
            if headding is not None:
                sys.stdout.write(headding + '\t')
            """ 1行分の出力 """
            for val in l:
                if type(val) is str:
                    sys.stdout.write(val + '\t')
                else:
                    print val, '\t',
            sys.stdout.write('\n')


if __name__ == "__main__":
    c = CTextFsm()
    exit(0)
