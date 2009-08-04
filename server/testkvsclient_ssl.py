#!/usr/bin/python
# Copyright (c) 2001-2004 Twisted Matrix Laboratories.
# See LICENSE for details.


from OpenSSL import SSL
import sys

from twisted.internet.protocol import ClientFactory
from twisted.protocols.basic import LineReceiver
from twisted.internet import ssl, reactor
from twisted.internet.protocol import Protocol

from kvsproto import *
import struct

class KVSTestClient(Protocol):
	def composePacket(self, cmd, data):
		return chr(cmd) + struct.pack(">I", len(data) + 5) + data
		
	def connectionMade(self):
		self.transport.write(self.composePacket(CMD_VALUESET, "thisisthekey\0HELLOWORLD"))
		
		self.transport.write(self.composePacket(CMD_VALUEGET, "thisisthekey"))
		

	def connectionLost(self, reason):
		print 'connection lost (protocol)'

	def dataReceived(self, line):
		s= ""
		for i in line:
			s+="%02x " % ord(i)
		print s
			
			
class KVSTestClientFactory(ClientFactory):
    protocol = KVSTestClient

    def clientConnectionFailed(self, connector, reason):
        print 'connection failed:', reason.getErrorMessage()
        reactor.stop()

    def clientConnectionLost(self, connector, reason):
        print 'connection lost:', reason.getErrorMessage()
        reactor.stop()

def main():
    factory = KVSTestClientFactory()
    reactor.connectSSL('localhost', 31423, factory, ssl.ClientContextFactory())
    reactor.run()

if __name__ == '__main__':
    main()
