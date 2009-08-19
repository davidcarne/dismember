#!/usr/bin/env python
import sys
import re
import os
import os.path


tested_files = set()
def check_file(path):
	global tested_files
	if path in tested_files: return True
	
	if os.access(path, 4):
		tested_files.add(path)
		return True
	return False

def parse_dep(path):
	line = open(path).readline()
	l = line.split(": ")
	if len(l) == 1: return []
	deps = l[1].split()
	return deps
	
def enumerate_deps(path):
	l = []
	header_matcher = re.compile("[a-zA-Z0-9_\-]*\.d$")

	for root, dirs, files in os.walk(path):
		for file in files:
			path = root +"/"+ file
			if (header_matcher.match(file)):
				l += [path]
	return l


depfiles = enumerate_deps("build")

for i in depfiles:
	file_deps = parse_dep(i)
	for j in file_deps:
		if not check_file(j):
			os.unlink(i)
			break