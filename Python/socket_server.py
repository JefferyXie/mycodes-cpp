#!/usr/bin/python
# -*- coding: utf-8 -*-

import socket

HOST = "localhost"
PORT = 1890
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
conn, addr = s.accept()
print 'Connected by', addr
while 1:
    data = conn.recv(1024)
    if not data: break
    data += '\\n**appended content from server**'
    conn.send(data)
#	conn.send('try 中文内容')
conn.close()

