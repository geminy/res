#!/usr/bin/env python

from twisted.internet import protocol, reactor

HOST = 'localhost'
PORT = 21567

class TSClntProtocol(protocol.Protocol):
	def sendData(self):
		data = raw_input('> ')
		if data:
			print '...sending %s...' %data
			self.transport.write(data)
		else:
			self.transport.loseConnection()

	def connectionMade(self):
		self.sendData()

	def dataReceived(self, data):
		print data
		self.sendData()

factory = protocol.ClientFactory()
factory.protocol = TSClntProtocol
reactor.connectTCP(HOST, PORT, factory)
reactor.run()