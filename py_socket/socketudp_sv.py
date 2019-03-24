#!/usr/local/bin/python
# -*- coding: utf-8 -*-
## !/usr/bin/python
''' refs
http://memo.saitodev.com/home/python_network_programing/#id1
server側(udp)
'''

import socket
import time

def run_server():
    host = '127.0.0.1'
    port = 4400
    backlog = 10
    buf_size = 4096
    cnt = 0

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind((host, port))
    print('Lisning %s[UDP:%d]' % (host,port))

    while True:
        cnt = cnt + 1
        rmsg, addr = sock.recvfrom(buf_size)
        smsg = '%d:%s' % (cnt, rmsg)
        print(smsg)

        if rmsg == "quit":
            break
#
    print("socket-closing .... wait 5 sec")
    time.sleep(5)
    sock.close()
    print("socket-closed")
    return

if __name__ == '__main__':
    run_server()
