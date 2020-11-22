#!/usr/local/bin/python
# -*- coding: utf-8 -*-
## !/usr/bin/python
""" 家系図を処理するモジュール

"""
# note    argv[0] argv[1]  argc = 2
# change  2017/06/16
#   入力ファイルを、カンマ区切りからＴＡＢ区切りへ変更
# change  2017/06/17
#   sison_makedigraph,Sosen_GraphTreeでNodeinfからソース名を取得しgv家系図を作成する
#   このため、nodeinfの何番目がソースファイル名かをあらわす、「FnaneIdx」を追加


import csv
import re
from optparse import OptionParser

__title__ = 'Kakeizu'
__version__ = '0.2.0'
__date__ = '2017/07/04'
__author__ = 'Hiroshi HARASAKI<mail@example.com>'
__license__ = ''
__copyright__ = 'Copyright (c) 2017 Hiroshi HARASAKI'
__status__ = 'developping'

## 家系図を作成するクラス
class Kakeizu(object):
    """ class Kakeizu. 家系図クラス
    
    家系図情報を保持する
    """
    def __init__(self):
        """ Constructor """

        # システム単位
        ## 検索の深さ最大
        self.maxdepth = 20
        ## Nodeinfで、ソースファイル名を表すidx
        self.fnameidx = 0
        ## 探索時、既出で中止するか(1)しないか(0)
        self.isdup = 1

        # edge
        ## エッジ情報:自分の子の一覧 sison[me] = "child1;child2;..;"
        self.sison = {}
        ## エッジ情報:自分の親の一覧 sosen[me] = "p1;p2;...;"
        self.sosen = {}
        ## エッジ情報:  edges = [ [p,c],[p,c],,,[p,c] ]
        self.edges = []
        ## 親の一覧（重複なし）	parents['hoge'] = 1 親に現れた
        self.parents = {}
        ## 子の一覧（重複なし）	childs['foo'] = 1 子に現れた
        self.childs = {}

        # nodeinf
        ## Nodeの情報（１ノード：１情報） nodeinf[me] = "key;val1;val2;val3"
        self.nodeinf = {}
        ## Nodeinfの見出し
        self.nodemidashi = ""

        # nodeinf2
        ## Nodeの情報（１ノードに複数情報可）。ＡＰ⇒ＤＢ／ＤＢ２／、、、
        self.nodeinf2 = {}
        ## Nodeinf2の見出し
        self.nodemidashi2 = ""

        # 指定された家系図毎
        ## 自分。探索の起点
        self.me = ""
        ## 検索方向 sison or sosen_makedigraph
        self.search = ""
        ## meの祖先 or 子孫の家系図
        self.kktree = []
        ## Nodeを探索したか否か
        self.visited = {}
        ## ＤＢを検索したか否か
        self.visited2 = {}

        # 2015.05.20 DSNの源流をたどる（祖先）
        ## DSN->JOBSTEPの祖先関係
        self.ds2js_sosen = {}
        ## JOBSTEP->DSNの祖先関係
        self.js2ds_sosen = {}

    def jobstep_read(self, fname):
        # :type (str)
        """ ジョブステップ毎のＤＤ文情報を読み込む
        :param str fname: ｃｓｖファイルのファイル名
        """
        # note
        #   ジョブステップ毎のＤＤ文情報をcsvファイルから読込み、
        #   self.Dsn2Jonstep,self.Js2Ds_Sosenに溜め込んでから、
        #   DSNの親子関係を、self.sison[] と self.sosen[] にためる

        csvfile = open(fname, 'rb')
        self.sison.clear()
        self.sosen.clear()
        self.parents.clear()
        self.childs.clear()
        self.edges = []

        self.ds2js_sosen.clear()
        self.js2ds_sosen.clear()

        # csvを読み込み、js2ds_sosen[], ds2js_sosen[]を作成する
        # JOB1
        # JSNAME
        #    DD DSN=DSN1,DISP=SHR ---> js2ds_sosen[job1_01]="dsn1;"
        #    DD DSN=DSN2,DISP=NEW ---> ds2js_sosen[dsn2]="job1_02;"
        #
        csvreader = csv.reader(csvfile, delimiter='\t')
        for row in csvreader:
            if len(row) != 5 or row[0][0:1] == '#':     # コメント＆空行
                continue
            else:
                jobname = row[0].strip()
                jsno = row[1].strip()
                jobstep = row[2].strip()
                dsn = row[3].strip()
                io = row[4].strip()

            # 入力DSNは、Jobstep -> Dsn の祖先
            if io == "I":
                key1 = jobname + "_" + jsno
                if key1 in self.js2ds_sosen:
                    self.js2ds_sosen[key1] += dsn + ";"
                else:
                    self.js2ds_sosen[key1] = dsn + ";"

            # 出力DSNは、Dsn -> Jobstep の祖先
            elif io == "O":
                key1 = dsn
                if key1 in self.ds2js_sosen:
                    self.ds2js_sosen[key1] += jobname + "_" + jsno + ";"
                else:
                    self.ds2js_sosen[key1] = jobname + "_" + jsno + ";"
            else:
                print "io not corrent", io
        csvfile.close()

        #
        # 出力ｄｓｎ⇒JOBSTEP⇒入力ｄｓｎをたどる
        #  出力ｄｓｎ(child)  -> 入力ｄｓｎ(parent)の祖先(.sosen[])と同時に
        #  入力ｄｓｎ(parent) -> 出力ｄｓｎ(child) の子孫(.sison[])を設定する
        #
        # JOB1
        # JSNAME
        #     DD DSN=DSN2,DISP=NEW ---> ds2js_sosen[dsn1]="job1_02;"
        #
        for dsn1 in self.ds2js_sosen:               # dsn1は出力ｄｓｎ
            child = dsn1
            if dsn1 == "":
                continue
            else:
                job_jsnos = self.ds2js_sosen[dsn1]  # dsn1 -> jobstep

            for job_jsno in job_jsnos.split(";"):
                if job_jsno == "":
                    continue
                else:
                    dsns = self.js2ds_sosen[job_jsno]  # jobstep -> dsn(s)
                for dsn in dsns.split(";"):
                    parent = dsn        # 入力ｄｓｎが一つ
                    if dsn == "":
                        continue

                    if child in self.sosen:
                        self.sosen[child] += parent + ";"
                    else:
                        self.sosen[child] = parent + ";"

                    edge = [parent, child]
                    self.edges.append(edge)
                    self.parents[parent] = 1
                    self.childs[child] = 1

                    if parent in self.sison:
                        self.sison[parent] += child + ";"
                    else:
                        self.sison[parent] = child + ";"

                    edge = [parent, child]
                    self.edges.append(edge)
                    self.parents[parent] = 1
                    self.childs[child] = 1

    def edge_read(self, fname, mode):
        # :type (str, str)
        """ エッジ情報をcsvファイルから読込み、self.sison[] と self.sosen[] にためる
        :param str fname: ｃｓｖファイルのファイル名
        :param str mode: PC=[親,子]の並び,CP=[子,親]の並び
        """
        csvfile = open(fname, 'rb')
        self.sison.clear()
        self.sosen.clear()
        self.parents.clear()
        self.childs.clear()
        self.edges = []

        csvreader = csv.reader(csvfile, delimiter='\t')
        for row in csvreader:
            if len(row) != 2 or row[0][0:1] == '#':		# コメント＆空行
                continue
            else:
                if mode == "PC":
                    parent = row[0].strip()
                    child = row[1].strip()
                elif mode == "CP":
                    child = row[0].strip()
                    parent = row[1].strip()
                else:
                    return -1

            if not parent:
                print "parent is null", row
                continue

            if not child:
                print "child is null", row
                continue

            if parent in self.sison:
                self.sison[parent] += child + ";"
            else:
                self.sison[parent] = child + ";"

            if child in self.sosen:
                self.sosen[child] += parent + ";"
            else:
                self.sosen[child] = parent + ";"

            edge = [parent, child]
            # Edgesにないとき、追加チェックを入れたら、相当遅くなったのでやめた
            # if edge not in self.edges :
            #    self.edges.append( edge )
            #    self.parents[ parent ] = 1
            #    self.childs[ child ]   = 1
            #
            self.edges.append(edge)
            self.parents[parent] = 1
            self.childs[child] = 1

        csvfile.close()

    def nodeinf_read(self, fname, itemslist, midashi):
        # :type (str, list, str)
        """ ノード情報をcsvファイルから読み、ディクショナリ self.nodeinf[]にためる
        :param str fname: csvフィルの名前
        :param list itemslist: csvファイルの何項目めを保持するかしているするリスト
                    [5,6,7]は5,6,7カラム(0-origne)を保持する
        :param str midashi: Nodeinfを表示する時の見出し
        """
        # note  一つのノードに、「；」でセパレートされた情報を次の文字列形式で保持
        # change    2017/06/30 Nodeinfの値に、keyを追加
        #   変更前 nodeinf[ key ] = "item1;item2;item3; ... ;"
        #   変更後 nodeinf[ key ] = "key;item1;item2;item3; ... ;"

        if fname is None or fname == "":
            return

        self.nodemidashi = midashi
        self.nodeinf.clear()
        csvfile = open(fname, 'rb')
        csvreader = csv.reader(csvfile, delimiter='\t')
        for row in csvreader:
            if not row or row[0][0:1] == '#':           # コメント＆空行
                continue
            else:
                key = row[0].strip()
                # 2017/06/30 変更
                # tmp = '' -> tmp = key
                tmp = key + ';'
                for item in itemslist:
                    tmp += row[item] + ';'
                self.nodeinf[key] = tmp
        csvfile.close()

    def nodeinf2_read(self, fname, itemslist, midashi):
        # :type (str, list, str)
        """ ノード情報をcsvファイルから読み、ディクショナリ self.nodeinf2[]にためる
        :param fname: nodeinf_readを参照
        :param itemslist:
        :param midashi:
        """
        # note   一つのノードに、「／」と「；」でセパレートされた情報を保持
        #   nodeinf2[ key ] = "item1_1;item1_2;item1_3; / item2-1;item2-2;item2-3; /"
        #   例  pgmid -> ＮＤＢ名;日本語;CRUD / ＮＤＢ名;日本語;CRUD /

        if fname is None or fname == "":
            return

        self.nodemidashi2 = midashi
        self.nodeinf2.clear()

        csvfile = open(fname, 'rb')
        csvreader = csv.reader(csvfile, delimiter='\t')
        for row in csvreader:
            if not row or row[0][0:1] == '#':		# コメント＆空行
                continue
            else:
                key = row[0].strip()
                tmp = ''
                for item in itemslist:
                    tmp += row[item] + ';'
                if key in self.nodeinf2:
                    self.nodeinf2[key] += tmp + '/'
                else:
                    self.nodeinf2[key] = tmp + '/'
        csvfile.close()

    def visited_inq(self):
        """ ディクショナリ self.visited[] にkeyがあるものを持つ文字列を返却する。
        その際に、先頭に見出し(nodemidashi)を付与する
        :return: self.meの家系図に出現した要素（親、子）を表示する文字列を返却
          nodemidashi
          me;nodeinf[ me ]
          key1;nodeinf[ key1 ]
          key2;nodeinf[ key2 ]
          keyN;nodeinf[ keyN ]
        """
        # change    2015/03/03 表示改善
        #   Visitedに自分が含まれるので、重複して出力されていたため、自分を除外
        #   if  key != me:  を追加
        # change    2017/06/30 Nodeinfの設定変更対応
        #   Nodeinfの0番目の要素に、自分自身のkeyを持つように変更したので、表示するのは、1番目以降の要素に変更

        kiten = self.me
        retval = self.nodemidashi + '\n'

        # 自分自身のnodeinfを表示
        retval += kiten
        if kiten in self.nodeinf:
            (node, nodeinf) = self.nodeinf[kiten].split(";", 1)
            retval += ";" + nodeinf
        retval += '\n'

        # 巡回したノードのnodeinfを、keyでソートして表示
        # nodeinfがなくても、巡回したノードは表示する
        for key in sorted(self.visited.keys()):
            if key != kiten:
                retval += "%s" % key  # 巡回したノードのkey
                # ノードのnodeinf
                if key in self.nodeinf:
                    (node, nodeinf) = self.nodeinf[key].split(";", 1)
                    retval += ";" + nodeinf
                retval += '\n'
        return retval

    def visited2_inq(self):
        """ ディクショナリ self.visited2[] にkeyがあるものを持つ文字列を返却する。
        その際に、先頭に見出し(nodemidashi2)を付与する
        :return: 家系図に出現したnodeinf2を表示する文字列を返却
          nodemidashi2
          key1;nodeinf2[ key1 ]
          key2;nodeinf2[ key2 ]
          keyN;nodeinf2[ keyN ]
        """

        retval = self.nodemidashi2 + '\n'

        for key in sorted(self.visited2.keys()):
            retval += "%s" % key
            retval += '\n'
        return retval

    ## sison_maketree の説明
    def sison_maketree(self, level, parent, kiten, stop=None):
        """ 自分の子孫を深さ優先で探索しリスト"kktree"を作成する
        [level,親,自分,err-flg]を self.kktree に追加し、
        自分の子供を再帰的に  [level+1,自分,子供,err-flg] 検索する
        :param int level: 検索レベル
        :param str parent: 親から自分が呼ばれた
        :param str kiten: 自分
        :param stop: 検索停止条件、正規表現
        :rtype:  int
        :return: 0 正常 / 1 検索停止あり
        """
        if level == 0:
            self.me = kiten
            self.kktree = []
            self.visited.clear()
            self.visited2.clear()

        # レベル０で、辞書なしは指定誤り
        if level == 0 and kiten not in self.sison:
            self.kktree.append([level, parent, kiten, 'NON'])
            return 1

        # 探索対象がヌル
        if not kiten:
            self.kktree.append([level, parent, kiten, 'NUL'])
            return 1

        # 一定以上の深さは、検索を中止する
        if level >= self.maxdepth:
            self.visited[kiten] = 1
            self.kktree.append([level, parent, kiten, 'MAX'])
            return 1

        # 検索済の場合は、中止する
        # 2017/06/30 フラグ＝1のみ、チェック実施
        if self.isdup != 0 and kiten in self.visited:
            self.kktree.append([level, parent, kiten, 'DUP'])
            return 1

        # 中止条件がある場合は、中止する
        if stop is not None and re.match(stop, kiten):
            self.visited[kiten] = 1
            self.kktree.append([level, parent, kiten, 'STP'])
            return 1

        # 自分の子供を再帰検索する
        self.visited[kiten] = 1
        self.kktree.append([level, parent, kiten, ''])

        level2 = level + 1
        if kiten in self.sison:
            for child in self.sison[kiten].split(';'):
                if child:
                    self.sison_maketree(level2, kiten, child, stop)

    def sison_disptree(self):
        """ リスト"kktree"を図式的に表示する。
        """
        # note
        #   Nodeinf2をしてCURDを表示し、最後に重複なしのアクセスTBL一覧を表示するため
        #   家系図(call-graph)を表示する時に作成しておく

        retval = ''
        for node in self.kktree:
            (level, parent, kiten, errflg) = node

            # 出力位置を調整し、自分の情報を出力する。
            if errflg != '':
                retval += "%03d;%s%s;%s" % (level, "_" * level, kiten, errflg)
                retval += "\n"
            else:
                retval += "%03d;%s%s" % (level, "_" * level, kiten)
                retval += "\n"
                # さらに、Nodeinf2を出力する
                if kiten in self.nodeinf2:
                    for dbinf in self.nodeinf2[kiten].split('/'):  # / で分解
                        if dbinf:
                            # 2015.04.27 add %s
                            retval += "   ;%s;%s%s" % ("", "_" * level, dbinf)
                            retval += "\n"
                            db = dbinf.split(';')[0]   # ; で分解
                            self.visited2[db] = 1
        return retval

    def sosen_maketree(self, level, child, kiten, stop=None):
        # :type (int str,str,str) -> int
        """ 自分の祖先を深さ優先で探索しリスト"kktree"を作成する
        [level,親,自分,err-flg]を self.kktree に追加し、
        自分の子供を再帰的に  [level+1,自分,子供,err-flg] 検索する
        :param int level: 検索レベル
        :param str child: 子から自分が呼ばれた
        :param str kiten: 自分
        :param stop: 検索停止条件、正規表現
        """
        if level == 0:
            self.me = kiten
            self.kktree = []
            self.visited.clear()
            self.visited2.clear()

        # レベル０で、辞書なしは指定誤り
        if level == 0 and kiten not in self.sosen:
            self.kktree.append([level, child, kiten, 'NON'])
            return 1

        # 探索対象がヌル
        if not kiten:
            self.kktree.append([level, child, kiten, 'NUL'])
            return 1

        # 一定以上の深さは、検索を中止する
        if level >= self.maxdepth:
            self.visited[kiten] = 1
            self.kktree.append([level, child, kiten, 'MAX'])
            return 1

        # 検索済の場合は、中止する
        # 2017/06/30 フラグ＝1のみ、チェック実施
        if self.isdup != 0 and kiten in self.visited:
            self.kktree.append([level, child, kiten, 'DUP'])
            return 1

        if stop is not None and re.match(stop, kiten):
            self.visited[kiten] = 1
            self.kktree.append([level, child, kiten, 'STP'])
            return 1

        # 自分の親を再帰検索する
        self.visited[kiten] = 1
        self.kktree.append([level, child, kiten, ''])

        level2 = level + 1
        if kiten in self.sosen:
            for parent in self.sosen[kiten].split(';'):
                if parent:
                    self.sosen_maketree(level2, kiten, parent, stop)

        return 0

    def sosen_disptree(self):
        """ リスト"kktree"を図式的に表示する。
        """

        retval = ''
        for node in self.kktree:
            (level, child, kiten, errflg) = node
            # 出力位置を調整し、自分の情報を出力する。
            if errflg != '':
                retval += "%03d;%s%s;%s" % (level, "_" * level, kiten, errflg)
                retval += "\n"
            else:
                retval += "%03d;%s%s" % (level, "_" * level, kiten)
                retval += "\n"
                # さらに、Nodeinf2を出力する
                if kiten in self.nodeinf2:
                    for dbinf in self.nodeinf2[kiten].split('/'):  # / で分解
                        if dbinf:
                            retval += "   ;%s%s" % ("_" * level, dbinf)
                            retval += "\n"
                            db = dbinf.split(';')[0]   # ; で分解
                            self.visited2[db] = 1
        return retval

    def options(self, usage):
        # :type (str) -> (dict, dict)
        """ コマンドラインを処理する
        optionsはオブジェクトなので、len()が使えない、is Noneを使う
        :param str usage: このモジュールのusage
        """
        parser = OptionParser(usage)
        # --------------
        #   オプション定義体の初期化
        # --------------
        #
        # --------------
        #   必須オプション
        # --------------
        # input, output directry
        parser.add_option("-i", "--indir", dest="indir",
                          help="input directory. EDGE, NODE, NODE2")
        parser.add_option("-o", "--otdir", dest="otdir",
                          help="output directory. used in gvkakeizu")
        # for edge file : name
        parser.add_option("-e", "--edge", dest="edge",
                          help="Graph EDGE data file. kbn=PC:Parent->Child, kbn=CP:Child->Parent")
        #
        # --------------
        #   任意オプション（デフォルト設定あり）
        # --------------
        # for edge kbn : PC or CP
        parser.add_option("--kbn", dest="kbn", default="PC",
                          help="EDGE file order, default is %default. PC:(call->called)!CP(called->call)")
        # for Execute, Search direction : sison or sosen
        # gvkakeizuは、urlでsison|sosenを指定するので、必須パラとはしない。
        parser.add_option("--search", dest="search", default="sison",
                          help="Search direction, sison(calls)|sosen(calle By). default is %default")
        # for etc property
        parser.add_option("--depth", dest="maxdepth", default=20, type="int",
                          help="Kakeizu search maximun depth. default is %default")
        parser.add_option("--fnidx", dest="fnameidx", default=0, type="int",
                          help="when making pdf-graph, nodeinf's xx.th row is used to be node-name. default is %default")
        parser.add_option("--isdup", dest="isdup", default=1, type="int",
                          help="when making kakeizu, if duplicated node, searching is abandon or not. default is yes(%default)")
        # --------------
        #   任意オプション（デフォルト設定なし） => 値Noneとなる場合あり
        # --------------
        # for nodeinf
        parser.add_option("--node", dest="node",
                          help="Node file(call,called). 1st row is key")
        parser.add_option("--col", dest="col",
                          help="Node's valid column number, Zero-origin, without key-column")
        parser.add_option("--titl", dest="titl",
                          help="Node's title. separate with TAB")
        # for nodeinf2
        parser.add_option("--node2", dest="node2",
                          help="Node2 file. 1st row is key")
        parser.add_option("--col2", dest="col2",
                          help="Node2's valid column number. Zero-origin, without key-column")
        parser.add_option("--titl2", dest="titl2",
                          help="Node2's title. separate with TAB")
        # stopper
        parser.add_option("--stop", dest="stop",
                          help="Search Stops at this regular expression")
        #
        (options, args) = parser.parse_args()
        #
        # --------------
        #   オプションのチェックと、内部変数への設定
        # --------------
        #
        # --------------
        #   引数チェク
        # --------------
        if len(args) != 1:
            parser.error("incorrect number of arguments")
        # --------------
        # 必須パラの指定有無チェック
        # --------------
        if options.otdir is None or options.indir is None or options.edge is None:
            parser.error("INDIR,OTDIR,EDGE,SEARCH is required")
        # --------------
        # 任意パラ＆デフォルトありの値チェック
        # --------------
        # kbn :
        if options.kbn != "PC" and options.kbn != "CP":
            parser.error("KBN must be PC or CP")
        # search : gvkakeizuは、urlでsison|sosenを指定するので、必須パラとはしない。
        if options.search != "sison" and options.search != "sosen":
            parser.error("SEARCH must be sison or sosen")
        else:
            self.search = options.search
        # maxdepath
        if options.maxdepth <= 0:
            parser.error("MAXDEPTH greater than 0")
        else:
            self.maxdepth = options.maxdepth
        # fnameidx
        if options.fnameidx < 0:
            parser.error("FNAMEIDX greater equal 0")
        else:
            self.fnameidx = options.fnameidx
        # isdup :
        if options.isdup != 0 and options.isdup != 1:
            parser.error("ISDUP must be 0 or 1")
        else:
            self.isdup = options.isdup

        return options,args

    def csvload(self, options):
        # :type (list)
        """ optionsで指定されたファイルからデータを読み込み、インスタンス変数に設定する。
        :param class options: Parserから作成さればオブジェクト
        """
        if options.edge is not None:
            edge = options.indir + "/" + options.edge
            self.edge_read(edge, options.kbn)

        if options.node is not None:
            node = options.indir + "/" + options.node
            col = self.str2list(options.col)
            self.nodeinf_read(node, col, options.titl)

        if options.node2 is not None:
            node2 = options.indir + "/" + options.node2
            col2 = self.str2list(options.col2)
            self.nodeinf2_read(node2, col2, options.titl2)

    def make_tree(self, search, start, stopper):
        # :type (str, str, str)
        """ 家系図ツリーを作成する
        :param str search: sison | sosen
        :param str start: 探索開始位置
        :param str stopper: 停止する正規表現
        """
        #
        if search == "sison":
            self.sison_maketree(0, "", start, stopper)
        elif search == "sosen":
            self.sosen_maketree(0, "", start, stopper)

    def print_cui(self):
        # :type(str)
        """ 家系図をCUI表示する
        """
        if self.search == "sison":
            print self.sison_disptree()
            print self.visited_inq()
            print self.visited2_inq()
        elif self.search == "sosen":
            print self.sosen_disptree()
            print self.visited_inq()
            print self.visited2_inq()

    def dbgprint(self):
        """ デバック情報表示 """

        print "<DBG>self.sison[]"
        for sison in sorted(self.sison):
            print sison, self.sison[sison]

        print "<DBG>self.sosen[]"
        for sosen in sorted(self.sosen):
            print sosen, self.sosen[sosen]

        print "<DBG>nodeinf"
        for node in sorted(self.nodeinf):
            pgminf = self.nodeinf[node]
            print node, pgminf

    @staticmethod
    def str2list(s):
        # :type(str) -> list
        """ strをカンマで区切りlistに変換する """
        l = []
        for x in s.split(','):
            l.append(int(x))
        return l

if __name__ == "__main__":
    KAKEIZU = Kakeizu()
    USAGE = "usage: %prog [options] start-point"
    (OPTS, ARGS) = KAKEIZU.options(USAGE)
    KAKEIZU.csvload(OPTS)
    #
    KAKEIZU.make_tree(OPTS.search, ARGS[0], OPTS.stop)
    KAKEIZU.print_cui()
    #
    # KAKEIZU.dbgprint()
