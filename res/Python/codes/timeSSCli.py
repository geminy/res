#!/usr/bin/env python

from socket import *

HOST = 'localhost'
PORT = 21567
BUFSIZE = 1024
ADDR = (HOST, PORT)

while True:
	tcpCliSock = socket(AF_INET, SOCK_STREAM) # socket every time
	tcpCliSock.connect(ADDR)
	data = raw_input('> ')
	if not data:
		break
	tcpCliSock.send('%s\r\n' %data)
	data = tcpCliSock.recv(BUFSIZE)
	if not data:
		break
	print data.strip()
	tcpCliSock.close()