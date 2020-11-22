#!/usr/local/bin/python
# -*- coding: utf-8 -*-
## !/usr/bin/python
""" 家系図をhttpで表示
:todo httpkakeizuはこのファイルで実行可能
"""
import BaseHTTPServer
import urllib2
import sys
from Kakeizu import Kakeizu

__title__ = 'Httpkakeizu'
__version__ = '0.2.0'
__date__ = '2017/07/04'
__author__ = 'Hiroshi HARASAKI<mail@example.com>'
__license__ = ''
__copyright__ = 'Copyright (c) 2017 Hiroshi HARASAKI'
__status__ = 'developping'


class MyHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    """ http版家系図
    BaseHTTPServer.BaseHTTPRequestHandlerの
    do_GET メソッドをオーバライドする。ブラウザからのリクエストを
    受け付ける処理を書く
    :todo  httpkakeizuをオブジェクト化したいが、未完のまま
    """
    #   body += 'COMMAND:' + self.command + ':' + '\n'
    #   body += 'PATH:'	   + self.path + ':' + '\n'
    #   body += 'VER:'	   + self.request_version + ':' + '\n'

    def do_GET(self):
        """ http-get-method
        httpからGETメソッドが発行されたときの動作
        """

        tmp = self.path[1:].split("/")
        if len(tmp) == 1:
            me = urllib2.unquote(tmp[0])
            cmd = ""
        else:
            cmd = tmp[0]
            me = urllib2.unquote(tmp[1])

        body = '<PRE>'
        if cmd == "sison":
            body += '---- sison [' + me + '] ----\n'
            KAKEIZU.sison_maketree(0, "", me)  # 家系図(kktree)の探索・作成
            body += KAKEIZU.sison_disptree()
        elif cmd == "sosen":
            body += '---- sosen [' + me + '] ----\n'
            KAKEIZU.sosen_maketree(0, "", me)  # 家系図(kktree)の探索・作成
            body += KAKEIZU.sosen_disptree()
        elif cmd == "exit":
            raise KeyboardInterrupt("Bye")
        else:
            body += 'request is invalid \n'
            body += 'ex. http://ipadress:port/sison/hogehoge' + '\n'
            body += 'ex. http://ipadress:port/sosen/hogehoge' + '\n'
        #
        body += '</PRE>'
        body += '<P>'

        body += '<PRE>'
        body += '----- Nodeinf1: Appeared nodes ----- : '
        if not KAKEIZU.visited:
            body += 'I(' + me + ') have no parent(child)\n'
        else:
            body += 'I(' + me + ') have ' + \
                    str(len(KAKEIZU.visited) - 1) + ' parent(child)\n'
        body += KAKEIZU.visited_inq()
        body += '</PRE>'

        if KAKEIZU.visited2:
            body += '<PRE>'
            body += '----- nodeinf2: nodes sub inf ----- : \n'
            body += KAKEIZU.visited2_inq()
            body += '</PRE>'

        self.send_response(200)
        self.send_header('Content-type', 'text/html; charset=utf-8')
        self.send_header('Content-length', len(body))
        self.end_headers()
        self.wfile.write(body)


if __name__ == "__main__":
    # Httpkakeizuの実行部
    #
    try:
        HOST = 'localhost'
        PORT = 8000

        KAKEIZU = Kakeizu()
        USAGE = "usage: %prog [options] start-point\n  when serve as httpp-server, start-points is null-str"
        (OPTS, ARGS) = KAKEIZU.options(USAGE)
        KAKEIZU.csvload(OPTS)
        #
        HTTPD = BaseHTTPServer.HTTPServer((HOST, PORT), MyHandler)
        print 'serving at port', PORT
        HTTPD.serve_forever()
        #
    except KeyboardInterrupt as msg:
        print 'serving end', msg.args
        sys.exit()
