#!/usr/bin/env python

from twisted.internet import protocol, reactor
from time import ctime

PORT = 21567

class TSServProtocol(protocol.Protocol):
	def connectionMade(self):
		self.clnt = self.transport.getPeer().host
		print '...connected from:', self.clnt
	def dataReceived(self, data):
		print '...received from [%s]\n\t%s' %(self.clnt, data)
		self.transport.write('[%s] %s' %(ctime(), data))

factory = protocol.Factory()
factory.protocol = TSServProtocol
print 'waiting for connection...'
reactor.listenTCP(PORT, factory)
reactor.run()