#!/usr/bin/python
# -*- coding: utf-8 -*-
## !/usr/bin/python
''' refs
http://memo.saitodev.com/home/python_network_programing/#id1
'''

import socket
import time
import sys

## backlog
BackLog = 10
## bufsize
BufSize = 4096

class   MyTcpClient(object):
    
    def __init__(self):
        """
        """
        ## server_ip
        self.server_ip = '0.0.0.0'
        ## server_port
        self.server_port = 0
        ## client_ip
        self.client_ip = '0.0.0.0'
        ## client_port
        self.client_port = 0
        ## socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def client_connect(self, ip, port):
        """
        """
        self.server_ip = ip
        self.server_port = port
        
        self.sock.connect((self.server_ip, self.server_port))
        (my_ip, my_port) = socket.getnameinfo(self.sock.getsockname(), socket.AF_INET)
        self.client_ip = my_ip
        self.client_port = my_port

    def client_sndrcv(self, smsg):
        self.sock.sendall(smsg)
        rmsg = self.sock.recv(BufSize).strip('\n').strip('\r')
        return rmsg
    
    def client_close(self):
        self.sock.close()
    

    def run_client(self, host, port):
        self.server_ip = host
        self.server_port = port

        while True:
            print("send:"),
            smsg = raw_input()
            smsg = smsg.strip('\n').strip('\r')
    
            if smsg == "close":
                print("__close__")
                print("socket-closing .... wait 5 sec")
                time.sleep(5)
                self.client_close()
                print("sock-closed")
                break
            elif smsg == "connect":
                print("__connect__")
                self.client_connect(self.server_ip, self.server_port)
                print("socket-connected to Cl[%s:%s] -> Sv[%s:%s]" % (self.client_ip,self.client_port,self.server_ip,self.server_port))
            else:
                print("__send__")
                rmsg = self.client_sndrcv(smsg)
                print(rmsg)
    #
        self.sock.close()


class   MyTcpServer(object):

    def __init__(self):
        ## server_ip
        self.server_ip = '0.0.0.0'
        ## server_port
        self.server_port = 0
        ## client_ip
        self.client_ip = '0.0.0.0'
        ## client_port
        self.client_port = 0
        ## socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def server_listen(self, ip, port):
        self.server_ip = ip
        self.server_port = port
        self.sock.bind((self.server_ip, self.server_port))
        self.sock.listen(BackLog)

    def server_connect(self):
        conn, address = self.sock.accept()
        self.client_ip = address[0]
        self.client_port = address[1]
        return conn


    def run_server(self, host, port):
        """
        :param host:str,
        :param port:int,
        """
        self.server_listen(host, port)
        print( 'Lisning %s[TCP:%d]' % (host, port) )
    
        # clientからのconnectを待つ
        print("waiting for connected")
        conn = self.server_connect()
        cnt = 0
        print("conn-connected from %s:%s" % (self.client_ip, self.client_port))
    
        while True:
            cnt = cnt + 1
            rmsg = conn.recv(BufSize)
            rmsg = rmsg.strip('\n')
            rmsg = rmsg.strip('\r')
            smsg = '%d %s:%d->%s:%d>%s' % (cnt, self.server_ip, self.server_port, self.client_ip, self.client_port, rmsg)
            print(smsg)
            try:
                conn.sendall(smsg + '\r' + '\n')
            except (socket.error) as e:
                print("exception catsh %s" % e)
                print("conn closeing")
                conn.close()
                # clientからのconnectを待つ
                print("conn-closed & waiting for connected")
                conn = self.server_connect()
                cnt = 0
                print("conn-connected from %s:%s" % (self.client_ip, self.client_port))
                rmsg = ""
    
            if rmsg == "quit":
                break
            elif rmsg == "cclose":
                print("conn-closing & socket-close .... wait 5 sec")
                time.sleep(5)
                conn.close()
                print("conn-closed")
                break
            elif rmsg == "crenew":
                print("conn-closing & reconnect .... wait 5 sec")
                time.sleep(5)
                conn.close()
                print("conn-closed & waiting for connected")
                # clientからのconnectを待つ
                self.server_connect()
                print("conn-connected")
    
    #
    #    sock.shutdown(SHUT_RDWR)
        print("socket-closing .... wait 5 sec")
        time.sleep(5)
        self.sock.close()
        print("socket-closed")
        return

if __name__ == '__main__':
    argc = len(sys.argv)
    ip = sys.argv[1] if argc > 1 else '127.0.0.1'
    port = sys.argv[2] if argc > 2 else '4000'
    print('run_server %s:%s" % (ip, port)')

    tcpserver = MyTcpServer()
    tcpserver.run_server(ip, int(port))
