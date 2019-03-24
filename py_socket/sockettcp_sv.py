#!/usr/local/bin/python
# -*- coding: utf-8 -*-
## !/usr/bin/python
''' refs
http://memo.saitodev.com/home/python_network_programing/#id1
server側(tcp)
'''

import socket
import time

def run_server():
    host = '127.0.0.1'
    port = 4000
    backlog = 10
    buf_size = 4096
    cnt = 0

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind((host, port))
    sock.listen(backlog)
    print( 'Lisning %s[TCP:%d]' % (host, port) )

    # clientからのconnectを待つ
    conn, address = sock.accept()
    print("conn-connected")

    while True:
        cnt = cnt + 1
        rmsg = conn.recv(buf_size)
        smsg = '%d:%s' % (cnt, rmsg)
        print(smsg)
        conn.sendall(smsg)

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
            conn, address = sock.accept()
            print("conn-connected")

#
#    sock.shutdown(SHUT_RDWR)
    print("socket-closing .... wait 5 sec")
    time.sleep(5)
    sock.close()
    print("socket-closed")
    return

if __name__ == '__main__':
    run_server()
