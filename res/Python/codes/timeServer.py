#!/usr/bin/env python

from socket import *
from time import ctime

HOST = '' # any available
PORT = 21567 # unused
BUFSIZE = 1024
ADDR = (HOST, PORT)

tcpSerSock = socket(AF_INET, SOCK_STREAM)
tcpSerSock.bind(ADDR)
tcpSerSock.listen(5) # max concurrence access

while True:
	print 'waiting for connection...'
	tcpCliSock, addr = tcpSerSock.accept() # blocking and passive
	print '...connected from: ', addr

	while True:
		data = tcpCliSock.recv(BUFSIZE)
		if not data:
			break
		tcpCliSock.send('[%s] %s' %(ctime(), data))

	tcpCliSock.close()

tcpSerSock.close() # optional and never exec