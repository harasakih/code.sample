#!/usr/local/bin/python
# -*- coding: utf-8 -*-
## !/usr/bin/python
""" 家系図をコンソールとpdfに表示
"""
import re
import sys
sys.path.append('/usr/local/lib/python2.7/site-packages/graphviz/')

from graphviz import Digraph
from Kakeizu import Kakeizu

__title__ = 'Gvkakeizu'
__version__ = '0.2.0'
__date__ = '2017/07/04'
__author__ = 'Hiroshi HARASAKI<mail@example.com>'
__license__ = ''
__copyright__ = 'Copyright (c) 2017 Hiroshi HARASAKI'
__status__ = 'complete'


class GvKakeizu(Kakeizu):
    """ 家系図をgraphvizを使ってpdf出力する """

    def __init__(self):
        """ Kakeizuの初期化とDigraphの初期化
        """

        Kakeizu.__init__(self)
        ## graphの出力format(pdf, png, svg,,,)
        self.format = 'svg'
        ## Digraphのオブジェクト
        self.diagraph = Digraph(comment='Kakeizu graph', format=self.format)

    def dot_init(self):
        """ Digraphの初期化 """
        ## Digraphのオブジェクト
        self.diagraph = Digraph(comment='Kakeizu graph', format=self.format)

    def sison_makedigraph(self, stop=None, gvformat=None):
        # :type (str,str)
        """ リスト"kktree"をdigraph.nodeに設定する,stopにマッチする共通ＳＲ配下は探索しない
        xxxx_maketree, xxxx_makedigraph, print_pdf
        :param stop: 検索中止条件、正規表現
        :param gvformat: graphvizの出力フォーマット
        """
        # 2017/06/17
        #   家系図をソースファイル名に変換して作成する
        #   nodeinfを参照しソースファイル名をnodeに定義する
        #
        if gvformat is not None:
            self.format = gvformat
        self.dot_init()
        for tree in self.kktree:
            (level, parent, child, errflg) = tree
            if level == 0:
                # level ZERO は、親がヌルなので、子のノードのみ追加
                if child in self.nodeinf:
                    pgminf = self.nodeinf[child]
                    socname = pgminf.split(";")[self.fnameidx]
                else:
                    socname = child
                self.diagraph.node(unicode(socname, 'utf-8'),
                                   fontname=u"ＭＳ ゴシック")
                continue

            if stop is not None and re.match(stop, parent):
                # 子孫の検索なので、自分が親になったら停止。
                continue

            if child in self.nodeinf:
                pgminf = self.nodeinf[child]
                soc_child = pgminf.split(";")[self.fnameidx]
            else:
                soc_child = child

            if parent in self.nodeinf:
                pgminf = self.nodeinf[parent]
                soc_parent = pgminf.split(";")[self.fnameidx]
            else:
                soc_parent = parent

            self.diagraph.node(unicode(soc_parent, 'utf-8'),
                               fontname=u"ＭＳ ゴシック")
            self.diagraph.node(unicode(soc_child, 'utf-8'),
                               fontname=u"ＭＳ ゴシック")
            self.diagraph.edge(unicode(soc_parent, 'utf-8'),
                               unicode(soc_child, 'utf-8'))

    def sosen_makedigraph(self, stop=None, gvformat=None):
        # :type (str, str)
        """リスト"kktree"をdigraph.nodeに設定する,stopにマッチする共通ＳＲ配下は探索しない
        xxxx_maketree, xxxx_makedigraph, print_pdf
        :param stop: 検索中止条件（正規表現）
        :param gvformat: graphvizの出力フォーマット
        """
        if gvformat is not None:
            self.format = gvformat
        self.dot_init()
        for node in self.kktree:
            (level, child, parent, errflg) = node
            if level == 0:
                if parent in self.nodeinf:
                    pgminf = self.nodeinf[parent]
                    socname = pgminf.split(";")[self.fnameidx]
                else:
                    socname = parent
                self.diagraph.node(unicode(socname, 'utf-8'),
                                   fontname=u"ＭＳ ゴシック")
                continue

            if stop is not None and re.match(stop, child):
                # 祖先の検索なので、自分が子になったら停止。
                continue

            if child in self.nodeinf:
                pgminf = self.nodeinf[child]
                soc_child = pgminf.split(";")[self.fnameidx]
            else:
                soc_child = child

            if parent in self.nodeinf:
                pgminf = self.nodeinf[parent]
                soc_parent = pgminf.split(";")[self.fnameidx]
            else:
                soc_parent = parent

            self.diagraph.node(unicode(soc_parent, 'utf-8'),
                               fontname=u"ＭＳ ゴシック")
            self.diagraph.node(unicode(soc_child, 'utf-8'),
                               fontname=u"ＭＳ ゴシック")
            self.diagraph.edge(unicode(soc_child, 'utf-8'),
                               unicode(soc_parent, 'utf-8'))

    def make_digraph(self, stopper):
        # :type (str, str)
        """ 家系図ツリーを作成する
        :param stopper: 停止する正規表現
        """
        if self.search == "sison":
            self.sison_makedigraph(stopper)
        elif self.search == "sosen":
            self.sosen_makedigraph(stopper)

    def print_pdf(self, wkdir):
        # :type str
        """ dotfileからPDFを作せする,
        :param wkdir: 出力ディレクトリ
        """
        if dir is not None:
            otdir = wkdir + "/"
        else:
            otdir = "./"
        #
        name = self.me
        flg = self.search
        dotname = otdir + name + '.' + flg + '.dot'
        otfile = open(dotname, 'w')
        print >> otfile, (self.diagraph.source.encode('utf-8'))
        self.diagraph.render(dotname, view=False)

    @staticmethod
    def gvnetwork_pdf(edges, name='GraphPdf', wkdir='./', gvformat='svg'):
        # :type (list, str, str, str)
        """ egdesからネットワークグラフのdotfileを作成する,出力先は wkdir/me.dot.pdf
        :param edges: ネットワーク図を作成する[from,to]のリスト
        :param name: ファイル名のタイトル
        :param wkdir: 出力先ディレクトリ
        :param gvformat: graphvizのフォーマット
        :return: zero
        """
        dot = Digraph(comment=name, format=gvformat)
        for edge in edges:
            (_from, _to) = edge
            dot.node(unicode(_from, 'utf-8'), fontname=u"ＭＳ ゴシック")
            dot.node(unicode(_to, 'utf-8'),   fontname=u"ＭＳ ゴシック")
            dot.edge(unicode(_from, 'utf-8'), unicode(_to, 'utf-8'))

        dotname = wkdir + name + '.dot'
        otfile = open(dotname, "w")
        print >> otfile, (dot.source.encode('utf-8'))
        otfile.close()
        dot.render(dotname, view=False)


if __name__ == "__main__":
    GVKAKEIZU = GvKakeizu()
    USAGE = "usage: %prog [options] start-point"
    (OPTS, ARGS) = GVKAKEIZU.options(USAGE)
    GVKAKEIZU.csvload(OPTS)
    #
    GVKAKEIZU.make_tree(OPTS.search, ARGS[0], OPTS.stop)
    GVKAKEIZU.format = 'pdf'
    GVKAKEIZU.make_digraph(OPTS.stop)
    #
    GVKAKEIZU.print_pdf(OPTS.otdir)
    #
    # GVKAKEIZU.print_cui()
    # GVKAKEIZU.dbgprint()
