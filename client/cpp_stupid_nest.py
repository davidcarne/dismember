import re
import sys
import TerminalController

term = TerminalController.TerminalController()

stdmatcher = re.compile("(std::\\w+\\s*)<[^<>]*>")
notepatcher = re.compile(" note: ")
errpatcher = re.compile(" error: ")

withmatcher = re.compile("\\[with[^\\[\\]]\\]")

i =  sys.stdin.readline()
while i:
	i = sys.stdin.readline()
	if (len(i) == 0): 
		continue
	file = i.split(":",1)
	
	print term.GREEN + file[0] + ":" + term.NORMAL,
	if (len(file) == 2):
		tofix = file[1]
		ol = len(tofix)
		nl = 0
		while (nl < ol):
			ol = len(tofix)
			tofix = stdmatcher.sub("\\1",tofix)
			#tofix = withmatcher.sub(tofix,"")
			nl = len(tofix)
		
		tofix = notepatcher.sub(term.YELLOW + " note" + term.NORMAL + ": ", tofix)
		tofix = errpatcher.sub(term.RED + " error" + term.NORMAL + ": ", tofix)
		print tofix.rstrip()