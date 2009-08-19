#!/usr/bin/env python

import os
import os.path
import re

excludes = ["hash_map.h", "elf32.h", "elf_common.h"]

def enumerate_headers(path):
	l = []
	header_matcher = re.compile("[a-zA-Z0-9_\-]*\.h$")

	for root, dirs, files in os.walk(path):
		for file in files:
			path = root +"/"+ file
			if (header_matcher.match(file) and path[0:9] != "./contrib" and file not in excludes):
				l += [path]
	return l
	
def check_header(path):
	header_matcher = re.compile("^#ifndef _([a-zA-Z0-9_]+)_[Hh]_$")
	lines = open(path).readlines()
	hdefs = [header_matcher.match(l) for l in lines if header_matcher.match(l)]
	
	if (len(hdefs) == 0):
		print path,"Missing include-once #ifndef"
		return
		
	if (len(hdefs) > 1):
		print path,"Found more than one #ifndef that matches template"
		for i in hdefs:
			print "\t",i.group(0)
		return
	if (len(hdefs) == 1):
		bn = os.path.basename(path)
		b = os.path.splitext(bn)
		b = b[0]
		match = hdefs[0].group(1)
		if (b.upper() != match.upper()):
			print "Incorrect include-once #ifndef", path,b.upper(),match.upper()
headers = enumerate_headers(".");

for i in headers:
	check_header(i);
