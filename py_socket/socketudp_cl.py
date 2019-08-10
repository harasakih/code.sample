#!/usr/bin/python
# -*- coding: utf-8 -*-
## !/usr/bin/python
''' refs
http://memo.saitodev.com/home/python_network_programing/#id1
client側(udp)
'''
from socketudp import MyUdpClient
import sys


if __name__ == '__main__':
    argc = len(sys.argv)
    ip = sys.argv[1] if argc > 1 else '127.0.0.1'
    port = sys.argv[2] if argc > 2 else '4400'
    print("run_client %s:%s" % (ip, port))
    
    udpclient = MyUdpClient()
    udpclient.run_client(ip, int(port))
