# Based on twisted SSL example

from OpenSSL import SSL

class SSLNetworkContextFactory:
	def __init__(self, keyfile):
		self.__keyfile = keyfile
	
	def getContext(self):
		ctx = SSL.Context(SSL.SSLv23_METHOD)
		ctx.use_certificate_file(self.__keyfile)
		ctx.use_privatekey_file(self.__keyfile)
		return ctx


if __name__ == '__main__':
	import kvs_server, sys
	import optparse
	
	parser = optparse.OptionParser();
	parser.add_option("-s", "--sslcert", dest="certname", help="use CERT as certificate", metavar = "CERT")
	parser.add_option("-p", "--port", type="int", dest="port", help="listen on PORT", metavar = "PORT", default=31423)
	
	
	(options, args) = parser.parse_args()
	
	if not options.certname:
		print "********************** WARNING - SSL NOT ENABLED *******************************"
	
	print options, args
	from twisted.internet.protocol import Factory
	from twisted.internet import ssl, reactor
	from twisted.python import log
	from kvs import KVS
	
	log.startLogging(sys.stdout)
	
	factory = Factory()
	factory.protocol = kvs_server.KVSServer
	factory.store = KVS(args[0])
	
	if options.certname:
		reactor.listenSSL(options.port, factory, SSLNetworkContextFactory(options.certname))
	else:
		reactor.listenTCP(options.port, factory)
	reactor.run()
	
	
