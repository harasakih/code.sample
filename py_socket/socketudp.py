#!/usr/bin/python
# -*- coding: utf-8 -*-
## !/usr/bin/python
''' refs
http://memo.saitodev.com/home/python_network_programing/#id1
server側(udp)
'''

import socket
import sys
import time

BufSize = 4096

class   MyUdpClient(object):
    def __init__(self):
        ## socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        ## server_ip,port
        self.server_ip = '0.0.0.0'
        self.server_port = 0
        ## client_ip,port
        self.client_ip = '0.0.0.0'
        self.client_port = 0

    def run_client(self, ip, port):
        self.server_ip = ip
        self.server_port = int(port)

        (my_ip, my_port) = socket.getnameinfo(self.sock.getsockname(), socket.AF_INET)
        self.client_ip = my_ip
        self.client_port = int(my_port)
        while True:
            print("send:"),
            smsg = raw_input()
            
            if smsg == "close":
                print("__close__")
                print("socket-closing .... wait 5 sec")
                time.sleep(5)
                self.sock.close()
                print("sock-closed")
                break
            else:
                print("__send__ Cl[%s:%d]->Sv[%s:%d]" % (self.client_ip, self.client_port, self.server_ip, self.server_port))
                self.sock.sendto(smsg, (self.server_ip, self.server_port))
    #
        self.sock.close()

class   MyUdpServer(object):

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
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        
    def run_server(self, ip, port):
        self.server_ip = ip
        self.server_port = port 

        cnt = 0
        self.sock.bind((self.server_ip, self.server_port))
        print('Lisning %s[UDP:%d]' % (self.server_ip, self.server_port))
    
        while True:
            cnt = cnt + 1
            rmsg, addr = self.sock.recvfrom(BufSize)
            self.client_ip = addr[0]
            self.client_port = addr[1]
            smsg = '%d %s:%d->%s:%d>%s' % (cnt, self.client_ip, self.client_port, self.server_ip, self.server_port,  rmsg)
            print(smsg)
    
            if rmsg == "quit":
                break
    #
        print("socket-closing .... wait 5 sec")
        time.sleep(5)
        self.sock.close()
        print("socket-closed")
        return

if __name__ == '__main__':
    argc = len(sys.argv)
    ip = sys.argv[1] if argc > 1 else '127.0.0.1'
    port = sys.argv[2] if argc > 2 else '4400'
    print('run_server UDP[%s:%s]" % (ip, port)')

    udpserver = MyUdpServer()
    udpserver.run_server(ip, int(port))
