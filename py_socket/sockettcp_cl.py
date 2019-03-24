#!/usr/local/bin/python
# -*- coding: utf-8 -*-
## !/usr/bin/python
''' refs
http://memo.saitodev.com/home/python_network_programing/#id1
client側(tcp)
'''
import socket
import sys
import time

def run_client():
    host = '127.0.0.1'
    port = 4000
    bufsize = 4096

    while True:
        print("send:"),
        smsg = raw_input()

        if smsg == "close":
            print("__close__")
            print("socket-closing .... wait 5 sec")
            time.sleep(5)
            sock.close()
            print("sock-closed")
            break
        elif smsg == "socket":
            print("__socket__")
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        elif smsg == "connect":
            print("__connect__")
            sock.connect((host, port))
            print("conn-connected")
        else:
            print("__send__")
            sock.sendall(smsg)
            print(sock.recv(bufsize))
#
    sock.close()


if __name__ == '__main__':
  run_client()
