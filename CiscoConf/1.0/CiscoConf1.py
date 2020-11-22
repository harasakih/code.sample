#!/usr/bin/python
# -*- coding: utf-8 -*-
##!/usr/bin/python
##!/usr/local/bin/python
""" Cisco 照会ログファイルを分割する

:Modify 20190730:
cmd'next_cdpnei'追加。func[next_cdpnei,get_allnext_cdpnei], class-object[self.edge_cdpnei]も追加

"""
import os
import sys
import re
import pprint
import ipaddress
from CTextFsm1 import CTextFsm
from CTextFsm1 import eInt
from CTextFsm1 import CiscoShrun
from enum import Enum


## eigrp topology
class eEigrpCode(Enum):
    """ for show ip eigrp output
    ルーティング情報の状態
    """
    P = 'Passive'  # EIGRP計算が終了
    A = 'Active'  # EIGRP計算が実行中
    U = 'Update'  # この宛先に、アップデートパケットを送信
    Q = 'Query'  # この宛先に、クエリーパケットを送信
    R = 'Reply'  # この宛先に、リプライパケットを送信


class eIprouCode(Enum):
    """  for show ip rou
    """
    L = 'local'
    C = 'connected'
    S = 'static'
    R = 'RIP'
    M = 'mobile'
    B = 'BGP'
    D = 'EIGRP'
    EX = 'EIGRP_external'
    O = 'OSPF'
    IA = 'OSPF_inter_area'

    N1 = 'OSPF_NSSA_external1'
    N2 = 'OSPF_NSSA_external2'
    E1 = 'OSPF_external1'
    E2 = 'OSPF_external2'
    i = 'IS-IS'
    su = 'IS-IS_summary'
    L1 = 'IS-IS_level1'
    L2 = 'IS-IS_level2'
    ia = 'IS-IS_inter_area'
    asta = 'candidate_default'
    U = 'per-user_static_route'
    o = 'ODR'
    P = 'periodic_downloaded_static_route'
    H = 'NHRP'
    l = 'LISP'
    plus = 'replicated_route'
    percent = 'next_hop_override'


class eCdpneiCode(Enum):
    """ sh cdp neibghor CODE
    """
    R = 'Router'
    T = 'Trans_Bridge'
    B = 'Source_Route_Bridge'
    S = 'Switch'
    H = 'Host'
    I = 'IGMP'
    r = 'Repeater'
    P = 'Phone'
    D = 'Remote'
    C = 'CVTA'
    M = 'Two-port_Mac_Relay'


cmd_common = ['get-cmd-common', 'logsplit_dirdir', 'logsplit_dir', 'logsplit', 'get-templ', 'get-mnem',
              'conf', 'confcsv', 'confcsv2', 'int2', 'rint',
              'netwild', 'ipinnet', 'ipinwild', 'ipinf', 'ipcalc', 'ip_int2',
              'grep', 'grep_shrunint']
cmd_route = ['get-cmd-route', 'get-eigrp-code', 'get-iprou-code', 'get-cdpnei-code',
             'ip_iprou', 'if_iprou', 'ip_eigrptop', 'if_eigrptop', 'ip_eigrptop2', 'if_eigrptop2',
             'nextip_eigrp', 'next_cdpnei'
             ]
cmd_acl = ['get-cmd-acl', 'get-acl', 'acls', 'test-acl', 'test-aclf']


# noinspection PyShadowingNames
class CiscoCommon(CTextFsm):
    """ CTextFsmを継承し、クラスを作成する
    """

    def __init__(self):
        """ CTextFsmの初期化
        """
        CTextFsm.__init__(self)
        ## shos_ip_eigrp_toplogyのFD_MINを保持
        ## shintの一覧。key=ファイル名(base)、valueは[ confname, interface, link_status, protocol_status, address, description, ip_address ]
        ## self.shint = {}

    @staticmethod
    def usage_common():
        argv0 = sys.argv[0]
        print('usage:%s CMD ARGS..' % os.path.basename(argv0))
        print('■準備')
        print('  get-cmd-common              : list available command')
        print('  logsplit_dirdir indir otdir : indir/DIRS/log to otdir/DIRS/log.split')
        print('  logsplit_dir    infiles     : infiles to current-dir')
        print('  logsplit logf               : ./logf to ./logf.split')
        print('  get-templ                   : template-names and template-file')
        print('  get-mnem                    : mnemonic symbos')
        print('■CiscoConfigの解析,by template')
        print('  conf     templname confnames : parse confname and pprint')
        print('  confcsv  templname confnames : parse confname and TSV-output')
        print('  confcsv2 templname confnames : parse confname ant TSV-output with confname')
        print('  int2     confnames(*.shint.txt) : parse int-summary to TSV with confname')
        print('  rint     confnames(*.shrun.txt) : parse int-shrun to TSV with confname')
        print('■IpAdress Calc')
        print('  netwild  addr wildmask      : wildmask to mask-len')
        print('  ipinnet  ip-addr net/l      : if ip-addr include net/l')
        print('  ipinwild ip-addr addr wild  : if ip-addr inclukde addr wild')
        print('  ipinf    net/l or net/wild  : net-addr   information')
        print('  ipcalc   SUBCMD ARGS..      : network address calculator')
        print('■ipcalc Subcommands')
        print('  ipcalc exclude   net1/l net2/l : net1 - net2')
        print('  ipcalc overlaps  net1/l net2/l : if net1 and net2 is not NULL')
        print('  ipcalc subnetsof net1/l net2/l : if net1 is subnet of net2')
        print('  ipcalc subnets   net1/l prefix_diff : divide net1 to ')
        print('■other')
        print('  ip_int2 ip-addr confname(*.shint.txt) : searhc ip-addr in shint')
        print('  grep regex confnames          : grep regex in confname')
        print('  grep_shrunint regex confnames(*.shrun.txt) : grep regex in shrun-int of confname')
        print('    ESCAPE is \\\\')

    def main(self):
        """ main
        """
        argc = len(sys.argv)
        basename = os.path.basename(sys.argv[0])
        dirname = os.path.dirname(sys.argv[0])

        cmd = sys.argv[1] if argc > 1 else None
        if cmd == '':
            usage_common()
            exit(1)
        elif cmd == 'get-cmd-common':
            pprint.pprint(cmd_common)
        elif cmd == 'get-templ':
            pprint.pprint(self.Templ_fname)
        elif cmd == 'get-mnem':
            pprint.pprint(self.Port_mnemonic)
        elif cmd == 'logsplit_dirdir':
            """ logsplit
            """
            if argc == 2:
                print('usage:%s %s indir otdir' % (basename, sys.argv[1]))
                exit(1)
            indir = sys.argv[2] if argc > 2 else './'
            otdir = sys.argv[3] if argc > 3 else './'
            self.logsplit_dirdir(indir, otdir)
        elif cmd == 'logsplit_dir':
            """ logsplit
            """
            if argc < 3:
                print('usage:%s %s infiles' % (basename, sys.argv[1]))
                exit(1)
            for logf in sys.argv[2:]:
                self.logsplit(logf, './')
        elif cmd == 'logsplit':
            """ logsplit
            """
            if argc < 3:
                print('usage:%s %s cisco-log' % (basename, sys.argv[1]))
                exit(1)
            logf = sys.argv[2]
            self.logsplit(logf)
        elif cmd == 'conf':
            """ TextFsmでparseした結果を、pprintで出力
            """
            if argc == 2:
                print('usage:%s %s templname confnames' % (basename, sys.argv[1]))
                exit(1)
            templ = sys.argv[2] if argc > 2 else None
            if not templ in self.Templ_fname:
                print('!ERR! templatename is not founnd:%s' % templ)
                exit(1)
            """ argv[3:]を処理する
            """
            for i in range(3, argc):
                confname = sys.argv[i]
                confobj = self.read_conf(confname)
                pprint.pprint(self.parse(confobj, templ))
        elif cmd == 'confcsv':
            """ TextFsmでparseした結果を、csv出力する
            = １カラム目をconfnameとしない
            = ファイル区切りでbasename(confname)を出力する
            """
            if argc < 4:
                print('usage:%s %s templname confnames' % (basename, sys.argv[1]))
                exit(1)
            templ = sys.argv[2] if argc > 2 else None
            if not templ in self.Templ_fname:
                print('!ERR! templatename is not founnd:%s' % templ)
                exit(1)
            """ 最初のファイル """
            confname = sys.argv[3]
            self.dbgprint(3, 'confcsv', os.path.basename(confname))
            print('# %s' % os.path.basename(confname))
            self.conf_csvprint(confname, templ, isheader=True, headding=None)
            """ ２つめ以降のファイル """
            for confname in sys.argv[4:]:
                self.dbgprint(3, 'confcsv', os.path.basename(confname))
                print('# %s' % os.path.basename(confname))
                self.conf_csvprint(confname, templ, isheader=True, headding=None)
        elif cmd == 'confcsv2':
            if argc < 4:
                print('usage:%s %s templname confnames' % (basename, sys.argv[1]))
                exit(1)
            """ template-name """
            templ = sys.argv[2] if argc > 2 else None
            if not templ in self.Templ_fname:
                print('!ERR! templatename is not founnd:%s' % templ)
                exit(1)
            """ headerをmodify """
            _header = CTextFsm.Csv_Header[templ]
            _header.insert(0, 'CONFNAME')
            """ 最初のファイル """
            confname = sys.argv[3]
            self.dbgprint(3, templ, os.path.basename(confname))
            base = self.confname2base(confname)
            parsedobj = self.set_parsed(confname, templ)
            self.csvprint(parsedobj, header=_header, headding=base)
            """ ２つめ以降のファイル """
            for confname in sys.argv[4:]:
                self.dbgprint(3, templ, os.path.basename(confname))
                base = self.confname2base(confname)
                parsedobj = self.set_parsed(confname, templ)
                self.csvprint(parsedobj, header=None, headding=base)
            pass
        elif cmd == 'int2':
            """ TextFsm(int)でparseした結果を、csv出力する。項目を選択。
            csv出力の１項目めは、self.confname2base(confname)
            """
            if argc < 3:
                print('usage:%s %s confname' % (basename, sys.argv[1]))
                exit(1)
            """ headerをmodify """
            _header = CTextFsm.Csv_Header['int2']
            _header.insert(0, 'CONFNAME')
            """ 最初のファイル """
            intobj2 = []
            confname = sys.argv[2]
            self.dbgprint(3, 'int2', os.path.basename(confname))
            base = self.confname2base(confname)
            intobj = self.set_parsed(confname, 'int')
            for _int in intobj:
                intobj2.append([
                    _int[eInt.interface],
                    _int[eInt.link_status],
                    _int[eInt.protocol_status],
                    _int[eInt.address],
                    _int[eInt.description],
                    _int[eInt.ip_address]
                ])
            self.csvprint(intobj2, header=_header, headding=base)
            """ ２つめ以降のファイル """
            for confname in sys.argv[3:]:
                intobj2 = []
                self.dbgprint(3, 'int2', os.path.basename(confname))
                base = self.confname2base(confname)
                intobj = self.set_parsed(confname, 'int')
                for _int in intobj:
                    intobj2.append([
                        _int[eInt.interface],
                        _int[eInt.link_status],
                        _int[eInt.protocol_status],
                        _int[eInt.address],
                        _int[eInt.description],
                        _int[eInt.ip_address]
                    ])
                self.csvprint(intobj2, header=None, headding=base)
            pass
        elif cmd == 'rint':
            """ sh run の結果から、interface情報を抽出する
            クラス「CiscoShrunのparse_shrunint」で解析する。
            csv出力の１項目めは、self.confname2base(confname)
            """
            if argc < 3:
                print('usage:%s %s confname' % (basename, sys.argv[1]))
                exit(1)
            """ headerをmodify """
            _header = CTextFsm.Csv_Header['rint']
            _header.insert(0, 'CONFNAME')
            ciscoshrun = CiscoShrun()
            """ 最初のファイル """
            confname = sys.argv[2]
            self.dbgprint(3, 'rint', os.path.basename(confname))
            base = self.confname2base(confname)
            parsedobj = ciscoshrun.parse_shrunint(confname)
            self.csvprint(parsedobj, header=_header, headding=base)
            """ ２つ目以降のファイル """
            for confname in sys.argv[3:]:
                self.dbgprint(3, 'rint', os.path.basename(confname))
                base = self.confname2base(confname)
                parsedobj = ciscoshrun.parse_shrunint(confname)
                self.csvprint(parsedobj, header=None, headding=base)
            pass
        elif cmd == 'netwild':
            if argc < 4:
                print('usage:%s %s address wildmask' % (basename, sys.argv[1]))
                exit(1)
            address = sys.argv[2] if argc > 2 else None
            wildmask = sys.argv[3] if argc > 3 else None
            is_address = self.is_ipaddress(address)
            is_wildmask = self.is_ipaddress(wildmask)
            if not (is_address and is_wildmask):
                self.errprint('netwild', '', ('address:%s wildmask:%s' % (is_address, is_wildmask)))
                exit(1)
            print address, wildmask, '->', self.ipwild2ip(address, wildmask)
        elif cmd == 'ipinnet':
            if argc < 4:
                print('usage:%s %s ip network' % (basename, sys.argv[1]))
                exit(1)
            ip = sys.argv[2] if argc > 2 else None
            net = sys.argv[3] if argc > 3 else None
            n = net.split('/')
            is_ip = self.is_ipaddress(ip)
            is_net = self.is_ipaddress(n[0])
            if not (is_ip and is_net):
                self.errprint('ipinnet', '', ('ip:%s network:%s' % (is_ip, is_net)))
                exit(1)
            if int(n[1]) < 0 or int(n[1]) > 32:
                self.errprint('ipinnet', '', 'network-mask not 0-32')
                exit(1)
            print ip, 'in', net, '->', self.is_ipinnet(ip, net)
        elif cmd == 'ipinwild':
            if argc < 5:
                print('usage:%s %s ip network wildmask' % (basename, sys.argv[1]))
                exit(1)
            ip = sys.argv[2] if argc > 2 else None
            net = sys.argv[3] if argc > 3 else None
            wild = sys.argv[4] if argc > 4 else None
            is_ip = self.is_ipaddress(ip)
            is_net = self.is_ipaddress(net)
            is_wild = self.is_ipaddress(wild)
            net2 = self.ipwild2ip(net, wild)
            if not (is_ip and is_net and is_wild):
                self.errprint('ipinwild', '', ('ip:%s network:%s wildmask:%s' % (is_ip, is_net, is_wild)))
                exit(1)
            print ip, 'in', net, wild, '(' + net2 + ')', '->', self.is_ipinnet(ip, net2)
        elif cmd == 'ipinf':
            if argc < 3:
                print('usage:%s %s network/wildmask' % (basename, sys.argv[1]))
                print('usage:%s %s network/masklen ' % (basename, sys.argv[1]))
                exit(1)
            network = sys.argv[2] if argc > 2 else None
            self.ipinf(network)
        elif cmd == 'ipcalc':
            if argc != 5:
                self.usage_ipcalc()
                exit(1)
            subcmd = sys.argv[2]
            net1 = ipaddress.IPv4Network(unicode(sys.argv[3], 'utf-8'))
            """ subcmdの処理
            """
            if subcmd == 'exclude':
                net2 = ipaddress.IPv4Network(unicode(sys.argv[4], 'utf-8'))
                print str(net1), 'address_exclude', str(net2)
                print list(net1.address_exclude(net2))
            elif subcmd == 'overlaps':
                net2 = ipaddress.IPv4Network(unicode(sys.argv[4], 'utf-8'))
                print str(net1), 'overlapse', str(net2)
                print net1.overlaps(net2)
            elif subcmd == 'subnetsof':
                net2 = ipaddress.IPv4Network(unicode(sys.argv[4], 'utf-8'))
                print str(net1), 'subnets_of', str(net2)
                print net1.subnet_of(net2)
            elif subcmd == 'subnets':
                diff = int(sys.argv[4])
                print str(net1), 'subnets', diff
                print list(net1.subnets(prefixlen_diff=diff))
            else:
                print('!ERR! SUBCMD invalid')
                self.usage_ipcalc()
                exit(1)
        elif cmd == 'ip_int2':
            ip = sys.argv[2] if argc > 2 else None
            for i in range(3, argc):
                confname = sys.argv[i]
                searched_int2 = self.search_ip_int2(ip, confname)
                """ 検索結果の出力
                """
                self.dbgprint(3, 'ip_int2', ('# search %s in int2[%s]' % (ip, confname)))
                self.print_int2(searched_int2)
        elif cmd == 'grep':
            regex = sys.argv[2] if argc > 2 else None
            for i in range(3, argc):
                confname = sys.argv[i]
                self._grep(regex, confname)
        elif cmd == 'grep_shrunint':
            regex = sys.argv[2] if argc > 2 else None
            for i in range(3, argc):
                confname = sys.argv[i]
                base = self.confname2base(confname)
                result = CiscoShrun.grepparse_shrunint(regex, confname)
                for x in result:
                    [interface, block] = x
                    print('%s in %s %s' % (regex, base, interface))
                    pprint.pprint(block)
        else:
            self.usage_common()
            exit(1)
        pass

    def logsplit_dirdir(self, indir=None, otdir=None):
        """ cisco-logを indir/DIRS/*.txt から otdir/DIRS/*.txt.ext.txtに分解

        :param indir :str, 入力ディレクトリ
        :param otdir :str, 出力ディレクトリ。otdir/DIRSはあらかじめ作成する必要がある。
        """
        for indir1 in os.listdir(indir):
            inpath = indir + '/' + indir1
            otpath = otdir + '/' + indir1
            for infname in os.listdir(inpath):
                m = re.match(r'^(.*)(_\d+_\d+).txt$', infname)
                if m is None:
                    self.dbgprint('logsplit_dirdir', '', 'Not Target file:' + inpath + ' ' + infname)
                else:
                    (base, els) = m.groups()
                    logf = inpath + '/' + infname
                    self.logsplit(logf, otdir=otpath, otfbase=base)
        pass

    @staticmethod
    def ipinf(ip_net):
        """ ip-address/maskの各種情報を出力

        :param ip_net :str, ip-address/mask
        """
        x = unicode(ip_net, 'utf-8')
        netaddr = ipaddress.IPv4Network(x, strict=False)
        print('network        :%s' % netaddr)
        print('network wild   :%s' % netaddr.with_hostmask)
        print('network_addr   :%s' % netaddr.network_address)
        print('broadcast_addr :%s' % netaddr.broadcast_address)
        print('hostmask       :%s' % netaddr.hostmask)
        print('netmask        :%s' % netaddr.netmask)
        print('num addressed  :%s' % netaddr.num_addresses)

    @staticmethod
    def print_int2(searched_ipint2):
        """ search_ip_int2の結果を出力する

        :param searched_ipint2 :list
        """
        for ipint2obj in searched_ipint2:
            [confname, int2] = ipint2obj
            [intf, linkst, protocolst, address, desc, ipaddress] = int2
            ## confname = self.confname2base(confname)
            print('%s: IF:%s LST:%s PST:%s N:%s D:%s' %
                  (confname, intf, linkst, protocolst, ipaddress, desc))
        pass

    def search_ip_int2(self, ip, confname):
        """ ipをint2から検索し、リストで返却

        :param  ip       :str, ip-address
        :param  confname :str, CiscoConfig file
        :return list [confname, [intf, linkst, protocolst, address, desc, ipaddress]]
        """
        confobj = self.read_conf(confname)  # .read()
        parsed = self.parse(confobj, 'int')  # parse with template
        rtn = []
        for _int in parsed:
            intf = _int[eInt.interface]
            linkst = _int[eInt.link_status]
            protocolst = _int[eInt.protocol_status]
            address = _int[eInt.address]
            desc = _int[eInt.description]
            net = _int[eInt.ip_address]
            if net != '':
                ip_address = ipaddress.IPv4Address(unicode(ip, 'utf-8'))
                network = ipaddress.IPv4Network(unicode(net, 'utf-8'), strict=False)
                _int2 = [intf, linkst, protocolst, address, desc, net]
                if ip_address in network:
                    rtn.append([confname, _int2])
        else:
            return rtn
        pass

    @staticmethod
    def _grep(regex, confname):
        """ grep

        :param regex    :str, regex, search string
        :param confname :str, CiscoConfig file
        """
        print('# grep %s in conf[%s]' % (regex, confname))
        i = 0
        with open(confname, mode='rt') as conf:
            for line in conf:
                i += 1
                l = line.rstrip()
                m = re.search(r'%s' % regex, l)  # 先頭に限らず検索
                if m is not None:
                    print('%4d: %s' % (i, l))
            pass
        pass

    @staticmethod
    def confname2base(confname):
        """ 拡張子を二つ削除する

        :param confname :str
        :return :str, basename(confname)から、拡張子を二つ削除
        """
        base = re.sub(r'\.[^.]+\.[^.]+$', '', os.path.basename(confname))
        return base


# noinspection PyTypeChecker,PyTypeChecker,PyTypeChecker
class CiscoRoute(CiscoCommon):
    """ CiscoのRouting情報を処理するクラス
    """

    def __init__(self):
        """ 継承したクラスの初期化
        """
        CiscoCommon.__init__(self)
        ### show_ip_eigrptopの最小値を保持
        self.minfd_ip_eigrptop = sys.maxint
        ## sho_ip_eigrp_toplogyのFD_MINとなった、eigrp情報
        self.minfd_ip_eigrptop_obj = []
        ## show_ip_routeのMETRIC_MINを保持
        self.minmt_ip_iprou = sys.maxint
        ## show_ip_routeのMETRIC_MINを保持となった、iprouを保持
        self.minmt_ip_iprou_obj = []
        ## show_ip_eigrp_topologyのconnectedとなっている、eigrpを保持
        self.connected_ip_eigrptop = []
        ## show_ip_routeのconnectedとなっている、iprouを保持
        self.connected_ip_iprou_obj = []
        ## EDGE : show_ip_eigrp_topologyの[asno + router, asno + nextip, cnt]
        self.router_nextip = []
        ## NODE : asno + routreの一覧。valueは検出したファイル名(base)。
        self.asno_router = {}
        ## EDGE(cdp_nei) : show_cdp_neighborの[from, to, cnt] のペア
        self.edge_cdpnei = []
        pass

    @staticmethod
    def usage_route():
        print('■準備')
        print('  get-cmd-route               : list available command')
        print('  get-eigrp-code              : Eigrp Code')
        print('  get-iprou-code              : Iprou Code')
        print('  get-cdpnei-code             : Cdpnei COde')
        print('■Routing(*.shiprou.txt,*.shipeigrptop.txtt)')
        print('  ip_iprou     ip-addr   confnames : search route contains ip-addr')
        print('  if_iprou     interface confnames : search interface')
        print('  ip_eigrptop  ip-addr   confnames : search route contains ip-addr')
        print('  if_eigrptop  interface confnames : search interface')
        print('  ip_eigrptop2 ip-addr   confnames : search route contains ip-addr, output 1 line')
        print('  if_eigrptop2 interface confnames : search interface, output 1 line')
        print('■Graphviz')
        print('  nextip_eigrp g-title confnames : make graph-path from eigrp-nei confname')
        print('  next_cdpnei  g-title confnames : make graph-path from cdp-nei confname')

    def main(self):
        """ main
        """
        argc = len(sys.argv)
        basename = os.path.basename(sys.argv[0])
        dirname = os.path.dirname(sys.argv[0])

        cmd = sys.argv[1] if argc > 1 else None
        if cmd == '':
            self.usage_route()
            exit(1)
        elif cmd == 'get-cmd-route':
            pprint.pprint(cmd_route)
        elif cmd == 'ip_eigrptop':
            ip = sys.argv[2] if argc > 2 else None
            for i in range(3, argc):
                confname = sys.argv[i]
                searched_eigrptop = self.search_ip_eigrptop(ip, confname)
                """ 検索結果の出力
                """
                self.dbgprint(3, 'ip_eigrptop', ('# search %s in eigrp[%s]' % (ip, confname)))
                self.print_eigrptop(searched_eigrptop, cr=True)
            else:
                """ 最小fdとなっているeigrpの出力
                """
                print('')
                print('IP:%s has min fd[%d] in...' % (ip, self.minfd_ip_eigrptop))
                self.print_eigrptop(self.minfd_ip_eigrptop_obj, cr=True)
                """ Connecedとなっているeigrpの出力
                """
                print('')
                print('IP:%s is connected in...' % ip)
                self.print_eigrptop(self.connected_ip_eigrptop, cr=True)
        elif cmd == 'ip_eigrptop2':
            ip = sys.argv[2] if argc > 2 else None
            for i in range(3, argc):
                confname = sys.argv[i]
                searched_eigrptop = self.search_ip_eigrptop(ip, confname)
                """ 検索結果の出力
                """
                self.dbgprint(3, 'ip_eigrptop2', ('# search %s in eigrp[%s]' % (ip, confname)))
                self.print_eigrptop(searched_eigrptop, cr=None)
            else:
                """ 最小fdとなっているeigrpの出力
                """
                print('')
                print('IP:%s has min fd[%d] in...' % (ip, self.minfd_ip_eigrptop))
                self.print_eigrptop(self.minfd_ip_eigrptop_obj, cr=None)
                """ Connecedとなっているeigrpの出力
                """
                print('')
                print('IP:%s is connected in...' % ip)
                self.print_eigrptop(self.connected_ip_eigrptop, cr=None)
        elif cmd == 'if_eigrptop':
            intf = sys.argv[2] if argc > 2 else None
            for i in range(3, argc):
                confname = sys.argv[i]
                searched_eigrptop = self.search_if_eigrptop(intf, confname)
                """ 検索結果の出力
                """
                self.dbgprint(3, 'if_eigrptop', ('# search %s in eigrp[%s]' % (intf, confname)))
                self.print_eigrptop(searched_eigrptop, cr=True)
        elif cmd == 'if_eigrptop2':
            intf = sys.argv[2] if argc > 2 else None
            for i in range(3, argc):
                confname = sys.argv[i]
                searched_eigrptop = self.search_if_eigrptop(intf, confname)
                """ 検索結果の出力
                """
                self.dbgprint(3, 'if_eigrptop2', ('# search %s in eigrp[%s]' % (intf, confname)))
                self.print_eigrptop(searched_eigrptop, cr=None)
        elif cmd == 'ip_iprou':
            ip = sys.argv[2] if argc > 2 else None
            for i in range(3, argc):
                confname = sys.argv[i]
                searched_iprou = self.search_ip_iprou(ip, confname)
                """ 検索結果の出力
                """
                self.dbgprint(3, 'ip_iprou', ('# search %s in iprou[%s]' % (ip, confname)))
                self.print_iprou(searched_iprou)
            else:
                """ 最小metricとなるiprouの出力
                """
                print('')
                print('IP:%s has min metric[%d] in...' % (ip, self.minmt_ip_iprou))
                self.print_iprou(self.minmt_ip_iprou_obj)
                """ connectedとなっているiprouの出力
                """
                print('')
                print('IP:%s is connected in...' % ip)
                self.print_iprou(self.connected_ip_iprou_obj)
        elif cmd == 'if_iprou':
            intf = sys.argv[2] if argc > 2 else None
            for i in range(3, argc):
                confname = sys.argv[i]
                searched_iprou = self.search_if_iprou(intf, confname)
                """ 検索結果の出力
                """
                self.dbgprint(3, 'if_iprou', ('# search %s in iprou[%s]' % (intf, confname)))
                self.print_iprou(searched_iprou)
        elif cmd == 'get-eigrp-code':
            for e in eEigrpCode:
                print e, e.value
        elif cmd == 'get-iprou-code':
            for e in eIprouCode:
                print e, e.value
        elif cmd == 'get-cdpnei-code':
            for e in eCdpneiCode:
                print e, e.value
        elif cmd == 'nextip_eigrp':
            if argc < 4:
                print('usage:%s %s graph-title confnames' % (basename, sys.argv[1]))
                exit(1)
            g_title = sys.argv[2] if argc > 2 else None
            self.get_allnextip_eigrp(sys.argv[3:])
            """ graphviz output
            """
            print('# edge')
            print('digraph %s {' % g_title)
            for x in self.router_nextip:
                _from = re.sub(r'\.', '_', x[0])
                _to = re.sub(r'\.', '_', x[1])
                print('  IP_%s -> IP_%s ;' % (_from, _to))
            print('# node')
            for key in self.asno_router.keys():
                _key = re.sub(r'\.', '_', key)
                _str1 = ('  IP_%s [label="' % _key)
                _str2 = '"] ;'
                print _str1 + self.asno_router[key] + _str2
            print('} # EOF')
        ## 20190730
        elif cmd == 'next_cdpnei':
            if argc < 4:
                print('usage:%s %s graph-title confnames' % (basename, sys.argv[1]))
                exit(1)
            g_title = sys.argv[2] if argc > 2 else None
            self.get_allnext_cdpnei(sys.argv[3:])
            """ graphviz output
            """
            print('# edge')
            print('digraph %s {' % g_title)
            for x in self.edge_cdpnei:
                """ change '-' to '_' """
                _from = re.sub(r'[\-\.]', '_', x[0])
                _to = re.sub(r'[\-\.]', '_', x[1])
                print('%s -> %s ;' % (_from, _to))
            print('# node')
            print('} # EOF')
        else:
            print('!ERR! CMD invalid')
            self.usage_route()
            exit(1)

    def search_ip_eigrptop(self, ip, confname):
        """ ipをeigrptopから検索し、リストで返却

        :param ip       :str, ip-address
        :param confname :str, CiscoConfig file
        :return [confname, [asno, router, code, route, route_mask, successors, fd, tag, serno, adv, outif]]
        """
        confobj = self.read_conf(confname)  # .read()
        parsed = self.parse(confobj, 'eigrptop')  # parse with template
        rtn = []
        for eigrp in parsed:
            [asno, router, code, route, route_mask, successors, fd, tag, serno, adv, outif] = eigrp
            net = route + '/' + route_mask
            ip_address = ipaddress.IPv4Address(unicode(ip, 'utf-8'))
            network = ipaddress.IPv4Network(unicode(net, 'utf-8'))
            if ip_address in network:
                """ 最小FDを持つ eigrp を検索
                """
                if fd == "Inaccessible" or fd == "Infinity":
                    continue  # Next eigrp
                ifd = 0 if fd == '' else int(fd)
                if ifd < self.minfd_ip_eigrptop:
                    self.minfd_ip_eigrptop = ifd
                    self.minfd_ip_eigrptop_obj = []
                    self.minfd_ip_eigrptop_obj.append([confname, eigrp])
                elif ifd == self.minfd_ip_eigrptop:
                    self.minfd_ip_eigrptop_obj.append([confname, eigrp])
                """ Connectedとなっているルータの検索
                """
                for via_nextip in adv:
                    nextip = via_nextip.split()
                    if nextip[0] == 'Connected':
                        self.connected_ip_eigrptop.append([confname, eigrp])
                """ 検索結果のスタック
                """
                rtn.append([confname, eigrp])
        else:
            return rtn
        pass

    def search_if_eigrptop(self, interface, confname):
        """ interfaceをeigrptopから検索し、リストで返却

        :param  interface :str, interface, regex
        :param  confname  :str, CiscoConfig file
        :return [confname, [asno, router, code, route, route_mask, successors, fd, tag, serno, adv, outif]]
        """
        confobj = self.read_conf(confname)  # .read()
        parsed = self.parse(confobj, 'eigrptop')  # parse with template
        rtn = []
        for eigrp in parsed:
            [asno, router, code, route, route_mask, successors, fd, tag, serno, adv, outif] = eigrp
            for x in outif:
                if re.match(interface, str(x)) is not None:
                    rtn.append([confname, eigrp])
        else:
            return rtn
        pass

    def search_ip_iprou(self, ip, confname):
        """ ipをiprouから検索し、リストで返却

        :param ip       :str, ip-address
        :param confname :str, CiscoConfig file
        :return [confname,[prot, type, route, route_mask, addist, metric, next_ip, next_if, uptime]]
        """
        confobj = self.read_conf(confname)  # .read()
        parsed = self.parse(confobj, 'iprou')  # parse with template
        rtn = []
        for iprou in parsed:
            [prot, type, route, route_mask, addist, metric, next_ip, next_if, uptime] = iprou
            net = route + '/' + route_mask
            ip_address = ipaddress.IPv4Address(unicode(ip, 'utf-8'))
            network = ipaddress.IPv4Network(unicode(net, 'utf-8'))
            """ 0.0.0.0/0 で default-gw を定義しているので、 0.0.0.0/0 の GW は検索対象外とする
            """
            if net != '0.0.0.0/0' and ip_address in network:
                """ 最小metricを持つiprouの検索
                """
                imetric = -1 if metric == '' else int(metric)
                if imetric < self.minmt_ip_iprou:
                    self.minmt_ip_iprou = imetric
                    self.minmt_ip_iprou_obj = []
                    self.minmt_ip_iprou_obj.append([confname, iprou])
                elif imetric == self.minmt_ip_iprou:
                    self.minmt_ip_iprou_obj.append([confname, iprou])
                """ connectedとなるiprouの検索
                """
                if prot == "C":
                    self.connected_ip_iprou_obj.append([confname, iprou])
                """ 検索結果のスタック
                """
                rtn.append([confname, iprou])
        else:
            return rtn
        pass

    def search_if_iprou(self, interface, confname):
        """ interfaceをiprouから検索し、stdoutに出力

        :param  interface :str, interface, regex
        :param  confname  :str, CiscoConfig file
        :return [confname,[prot, type, route, route_mask, addist, metric, next_ip, next_if, uptime]]
        """
        confobj = self.read_conf(confname)  # .read()
        parsed = self.parse(confobj, 'iprou')  # parse with template
        rtn = []
        for iprou in parsed:
            [prot, type, route, route_mask, addist, metric, next_ip, next_if, uptime] = iprou
            net = route + '/' + route_mask
            if re.match(interface, next_if) is not None:
                rtn.append([confname, iprou])
        else:
            return rtn
        pass

    def nextip_eigrptop(self, confname):
        """ confnameをeigrptopで解析し、自分->次ルータを検索しdigraph作成用オブジェクトを設定する

        一つのconfname(=router)を処理する
            for node : self.asno_router[(asno, router)] = self.confname2base(confname)を設定
            for edge : NONAME-DIC[(asno,router,_nextip)] = cnt を返却
              router is from, _netxip is to

        :param  confname :str, a confname
        :return :dic, key=(asno, router, nextip), val=count in confname
        """
        confobj = self.read_conf(confname)  # .read()
        parsed = self.parse(confobj, 'eigrptop')  # parse with template
        rtn = {}
        _asno = ''
        _router = ''
        for eigrp in parsed:
            [asno, router, code, route, route_mask, successors, fd, tag, serno, adv, outif] = eigrp
            if _asno != asno or _router != router:
                key = '%s_%s' % (asno, router)
                if key in self.asno_router:
                    fname = os.path.basename(confname)
                    _msg = 'Key dup\n%s->%s in %s' % \
                           (key, self.asno_router[key], fname)
                    self.errprint('nextip_eigrptop', 'CiscoConf', _msg)
                pass
                self.asno_router[key] = self.confname2base(os.path.basename(confname))
                _asno = asno
                _router = router
            pass
            for via_nextip in adv:
                nextip = via_nextip.split()  # via_nextip is '\S+' or 'ip-addr (addis/metric)'
                _nextip = nextip[0]
                if re.match(r'\d+\.\d+\.\d+\.\d+', _nextip) is not None:
                    if not (asno, router, _nextip) in rtn:
                        rtn[(asno, router, _nextip)] = 1
                    else:
                        rtn[(asno, router, _nextip)] += 1
            pass
        pass
        return rtn

    def get_allnextip_eigrp(self, confnames):
        """ confnamesを順にeigrptopで解析し、self.router_nextip[]を設定する。

        self.asno_router{}はnextip_eigrptop(confname)で設定する。

        :param  confnames :list
        :return :list, [[asno+ip, asno+nextip, cnt],[],]
        """

        alldic = {}
        rtn = []
        self.asno_router = {}
        self.router_nextip = []
        for confname in confnames:
            dic = self.nextip_eigrptop(confname)
            """ 一つのconfnameの処理が終わったら、alldicに設定する
            """
            for key in dic.keys():
                (asno, ip, nextip) = key
                cnt = dic[key]
                if key in alldic:
                    alldic[key] += cnt
                else:
                    alldic[key] = cnt
        pass
        """
        全てのファイルの処理を終えたので、self.router_nextip[]を設定する
        """
        for key in sorted(alldic.keys()):
            (asno, ip, nextip) = key
            cnt = alldic[key]
            _from = '%s_%s' % (asno, ip)
            _to = '%s_%s' % (asno, nextip)
            rtn.append([_from, _to, cnt])
        pass
        self.router_nextip = rtn
        return rtn

    def next_cdpnei(self, confname):
        """ confnameをcdpneiで解析し、自分->次ルータを検索しdigraph作成用オブジェクトを設定する

        :param  confname :str, a confname
        :return :dic, key=(from, to), val=count in confname
        """
        confobj = self.read_conf(confname)  # .read()
        parsed = self.parse(confobj, 'cdpnei')  # parse with template
        rtn = {}
        base = self.confname2base(os.path.basename(confname))
        fname = os.path.basename(confname)
        for cdpnei in parsed:
            [neighbor, local_interface, capability, platform, neighbor_interface] = cdpnei
            key = base, neighbor
            if key in rtn:
                _msg = 'Key dup\n%s->%s in %s' % (base, neighbor, fname)
                self.errprint('next_cdpnei', 'CiscoConf', _msg)
                rtn[key] += 1
            else:
                rtn[key] = 1
        return rtn

    def get_allnext_cdpnei(self, confnames):
        """ confnamesを順にcdpneiで解析し、self.cdpnei_next[]を設定する。

        :param  confnames :list
        :return :list, [[from, to, cnt],[],]
        """

        alldic = {}
        rtn = []
        self.edge_cdpnei = []
        for confname in confnames:
            dic = self.next_cdpnei(confname)
            """ 一つのconfnameの処理が終わったら、alldicに設定する
            """
            for key in dic.keys():
                (_from, _to) = key
                cnt = dic[key]
                if key in alldic:
                    alldic[key] += cnt
                else:
                    alldic[key] = cnt
        pass
        """
        全てのファイルの処理を終えたので、self.cdpnei_next[]を設定する
        """
        for key in sorted(alldic.keys()):
            (_from, _to) = key
            cnt = alldic[key]
            rtn.append([_from, _to, cnt])
        pass
        self.edge_cdpnei = rtn
        return rtn

    @staticmethod
    def print_eigrptop(searched_eigrptop, cr=True):
        """ search_ip_eigrptopの結果を出力する

        :param searched_eigrptop :list
        :param cr                :bool, if CR or not, when adv, outif is listed
        """
        for eigrptop in searched_eigrptop:
            [confname, eigrp] = eigrptop
            [asno, router, code, route, route_mask, successors, fd, tag, serno, adv, outif] = eigrp
            net = route + '/' + route_mask
            ## confname = self.confname2base(confname)
            if cr is True:
                print('%s: A:%s R:%s C:%s RT:%s FD:%s\n  %s\n  %s' %
                      (confname, asno, router, code, net, fd, map(str, adv), map(str, outif)))
            else:
                print('%s: A:%s R:%s C:%s RT:%s FD:%s %s %s' %
                      (confname, asno, router, code, net, fd, map(str, adv), map(str, outif)))
        pass

    @staticmethod
    def print_iprou(searched_iprou):
        """ search_ip_iprouの結果を出力する

        :param searched_iprou :list
        """
        for iprouobj in searched_iprou:
            [confname, iprou] = iprouobj
            [prot, type, route, route_mask, addist, metric, next_ip, next_if, uptime] = iprou
            net = route + '/' + route_mask
            ## confname = self.confname2base(confname)
            print('%s: P:%s RT:%s DS:%s ME:%s NIP:%s IF:%s' %
                  (confname, prot + type, net, addist, metric, next_ip, next_if))
        pass


class CiscoAcl(CiscoCommon):
    """ CiscoのACLを処理するクラス
    """

    def __init__(self):
        """ 継承したクラスの初期化
        """
        CiscoCommon.__init__(self)

    @staticmethod
    def usage_acl():
        print('■ACL(*.shipacl.txt)')
        print('  get-cmd-acl                      : list available command')
        print('  acls     confnames(*.shipacl.txt): get access-lists-summary to TSV')
        print('  get-acl  confname [acl-name]     : get a access-list from confname')
        print('  test-acl confname [testpattern]  : testpattern check by confname')
        print('  test-aclf confname testfile      : test-acl from FILE')
        print('    testpattern is like.')
        print('    aclname protocol src-ip src-port dst-ip dst-port')

    def main(self):
        """ main
        """
        argc = len(sys.argv)
        basename = os.path.basename(sys.argv[0])
        dirname = os.path.dirname(sys.argv[0])

        cmd = sys.argv[1] if argc > 1 else None
        if cmd == '':
            self.usage_acl()
            exit(1)
        elif cmd == 'get-cmd-acl':
            pprint.pprint(cmd_acl)
        elif cmd == 'get-acl':
            """ confnameからaclnameを抽出し、csv出力する
            """
            if argc == 2:
                print('usage:%s %s confname [aclname]' % (basename, sys.argv[1]))
                exit(1)
            confname = sys.argv[2] if argc > 2 else None
            aclname = sys.argv[3] if argc > 3 else None
            confobj = self.read_conf(confname)  # .read()
            parsed = self.parse_acl(confobj)  # parse with template
            aclobj = self.parsed2acl(parsed)  # parsed-acl to aclobj{'aclname'}
            header = self.Csv_Header['net4']
            if aclname is None:
                for _aclname in aclobj:
                    acls = aclobj[_aclname]
                    print('access-list[%s]' % _aclname)
                    self.csvprint(acls, header)
            else:
                _aclname = aclname
                acls = aclobj[aclname]
                self.csvprint(acls, header)
            pass
        elif cmd == 'acls':
            """ confnameからaclを抽出し、csv出力する
            csv出力の１項目めは、self.confname2base(confname)
            """
            if argc < 3:
                print('usage:%s %s confname' % (basename, sys.argv[1]))
                exit(1)
            """ headerをmodify """
            _header = CTextFsm.Csv_Header['net4']
            _header.insert(0, 'CONFNAME')
            """ headerだけ出力 """
            self.csvprint([], _header, None)
            """ 最初のファイル """
            confname = sys.argv[2]
            self.dbgprint(3, 'acls', os.path.basename(confname))
            base = self.confname2base(confname)
            confobj = self.read_conf(confname)
            parsed = self.parse_acl(confobj)  # parse with template
            aclobj = self.parsed2acl(parsed)  # parsed to aclobj
            for key in sorted(aclobj.keys()):
                self.csvprint(aclobj[key], None, base)
            """ ２つめ以降のファイル """
            for confname in sys.argv[3:]:
                self.dbgprint(3, 'acls', os.path.basename(confname))
                base = self.confname2base(confname)
                confobj = self.read_conf(confname)
                parsed = self.parse_acl(confobj)  # parse with template
                aclobj = self.parsed2acl(parsed)  # parsed to aclobj
                for key in sorted(aclobj.keys()):
                    self.csvprint(aclobj[key], None, base)
        elif cmd == 'test-acl':
            if argc < 3:
                print('usage:%s %s confname [test-pattern]' % (basename, sys.argv[1]))
                exit(1)
            confname = sys.argv[2] if argc > 2 else None
            test = sys.argv[3] if argc > 3 else None
            confobj = self.read_conf(confname)  # .read()
            parsed = self.parse_acl(confobj)  # parse with template
            aclobj = self.parsed2acl(parsed)  # parsed-acl to aclobj{'aclname'}
            header = self.Csv_Header['net4']
            """ test-pattern未指定時は、標準入力でループ
            """
            if test is None:
                while test != 'quit':
                    test = raw_input('test-acl>')
                    if test == 'quit':
                        break  # Breakout while-loop
                    else:
                        [tf, name, action, list] = self.ismatch_acl(test, aclobj)
                        print tf, action,
                        for l in list:
                            print ':' + l,
                        print ''
                pass
                print 'bye bye'
            else:
                [tf, name, action, list] = self.ismatch_acl(test, aclobj)
                print tf, action,
                for l in list:
                    print ':' + l,
                print ''
        elif cmd == 'test-aclf':
            if argc != 4:
                print('usage:%s %s confname test-file' % (basename, sys.argv[1]))
                exit(1)
            confname = sys.argv[2] if argc > 2 else None
            testfile = sys.argv[3] if argc > 3 else None
            confobj = self.read_conf(confname)  # .read()
            parsed = self.parse_acl(confobj)  # parse with template
            parsed = self.set_acl(confname)
            aclobj = self.parsed2acl(parsed)  # parsed-acl to aclobj{'aclname'}
            #
            with open(testfile, mode='rt') as testf:
                for line in testf:
                    test = line.rstrip()  # strip \n
                    if re.match(r'^\s*$', test) is not None:
                        pass
                    elif re.match(r'^#.*$', test) is not None:
                        print test
                    else:
                        [tf, name, action, list] = self.ismatch_acl(test, aclobj)
                        print test, '->',
                        for l in list:
                            print ':' + l,
                        print ''
                    pass
            pass
        else:
            print('!ERR! CMD invalid')
            self.usage_acl()
            exit(1)
        pass


##
if __name__ == '__main__':
    common = CiscoCommon()
    common.DbgLevel[1] = 0
    common.DbgLevel[2] = 0
    common.DbgLevel[3] = 1

    argc = len(sys.argv)
    basename = os.path.basename(sys.argv[0])
    dirname = os.path.dirname(sys.argv[0])
    #   CTextFsm.Ntc_Dir = '/usr/lib/python2.7/site-packages/ntc_templates-1.1.0-py2.7.egg/ntc_templates/templates/'
    CTextFsm.Ntc_Dir = dirname + '/ntc_templates/'

    cmd = ''
    if argc < 2:
        CiscoCommon.usage_common()
        CiscoRoute.usage_route()
        CiscoAcl.usage_acl()
        exit(1)
    else:
        cmd = sys.argv[1]

    if cmd is '':
        CiscoCommon.usage_common()
        CiscoRoute.usage_route()
        CiscoAcl.usage_acl()
        exit(1)
    elif cmd in cmd_common:
        common = CiscoCommon()
        common.main()
    elif cmd in cmd_route:
        route = CiscoRoute()
        route.main()
    elif cmd in cmd_acl:
        acl = CiscoAcl()
        acl.main()
    else:
        CiscoCommon.usage_common()
        CiscoRoute.usage_route()
        CiscoAcl.usage_acl()
        exit(1)
    pass
    exit(0)
