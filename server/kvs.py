
# Temporary Naive implementation
class KVS(object):
	def __init__(self, filename):
		self.__kvdict = {}
		
	def setKey(self, key, value):
		self.__kvdict[key] = value
	
	def getKey(self, key):
		return self.__kvdict[key]