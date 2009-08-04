from twisted.internet.protocol import Protocol, Factory
from twisted.internet import reactor, defer
from collections import deque
import struct


from kvsproto import *

class KVSServer(Protocol):
	def __init__(self):
		self.__pktbuf = []
		self.__authenticated = 0
		self.__username = ""
	
	def composePacket(self, cmd, data):
		return chr(cmd) + struct.pack(">I", len(data) + 5) + data
		
	def packetHandlePING(self, data):
		return self.composePacket(CMD_PONG, data)
		
	def packetHandleAUTHVAL(self, data):
		username =  data[:data.find('\0')]
		hhash = data[data.find('\0')+1:]
		self.__authenticated = 1
		self.__username = data[:data.find('\0')]
		return self.composePacket(CMD_AUTHOK, chr(0))
		
	def packetHandleVALUESET(self, data):
		(path, value) = data.split('\x00')
		self.factory.store.setKey(path, value)
		
		
	def packetHandleVALUEGET(self, path):
		return self.composePacket(CMD_VALUESET, path + '\x00' + self.factory.store.getKey(path))
		
	def packetHandleROLLUPKEYS(self, data):
		return ""
		
	def packetHandleQTRANSNUM(self, data):
		return ""
	
	cmds = { CMD_PING: packetHandlePING,
			 CMD_PONG: packetHandleAUTHVAL,
			 CMD_QTRANSNUM: packetHandleQTRANSNUM,
			 CMD_VALUESET: packetHandleVALUESET,
			 CMD_VALUEGET: packetHandleVALUEGET,
			 CMD_ROLLUPKEYS: packetHandleROLLUPKEYS
			 };
			 
	def packetHandler(self, cmd, data):
		try:
			cb = self.cmds[cmd]
		except KeyError:
			print "Unhandled command %x" % cmd
			return ""
		return cb(self, data)
		
	def processData(self):
		while len(self.__pktbuf) >= 5: 
		
			cmd = ord(self.__pktbuf[0])
			mlen, = struct.unpack(">I", "".join(self.__pktbuf[1:5]))
			print "CMD = %x MLEN = %x SZ = %x" %(cmd, mlen, len(self.__pktbuf))
			if len(self.__pktbuf) >= mlen:
				data = self.__pktbuf[5:mlen]
				self.__pktbuf = self.__pktbuf[mlen:]
				defer.succeed(self.packetHandler(cmd, "".join(data))).addCallback(lambda m: self.transport.write(m))
			else:
				break
				
	def dataReceived(self, data):
		self.__pktbuf.extend(data)
		self.processData()
		