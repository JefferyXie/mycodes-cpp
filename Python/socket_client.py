#!/usr/bin/env python
# -*- coding: utf-8 -*-

# or change above line with #!/usr/bin/python

import socket

HOST = 'localhost' # the remote host server
PORT = 1890 # the same port as used by server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))
#s.send('Hello world, python 中文字')
s.send("hello world, python")
data = s.recv(1024)
s.close()
print 'Received', repr(data)
