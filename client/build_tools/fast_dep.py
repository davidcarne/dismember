#!/usr/bin/env python
import sys
import re
import os
import os.path

include_paths = []
for i in sys.argv:
	if i[0] == '-':
		if i[1] == 'I':
			include_paths += [i[2:]]
	else:
		file = i

#include_paths = [i for i in include_paths if i[0] != '/']			
include_paths = set(include_paths)
include_paths = [i for i in include_paths]

include_known_files = {}

already_checked_files = []

for i in include_paths:
	x = [(j,i +"/"+ j) for j in os.listdir(i)]
	for i in x:
		include_known_files[i[0]] = i[1]
		
#print include_known_files
include_matcher = re.compile('#include\s+"([0-9a-zA-Z_\-\./]+)"')

def get_includes(file):
	return [include_matcher.match(i).group(1) for i in open(file).readlines() if include_matcher.match(i)]

def find_include_path(cpath, name):
	
	global include_known_files
	if name in include_known_files: return include_known_files[name]
	
	for i in include_paths + [os.path.dirname(cpath)]:
		try:
			tryname = i + "/" + name
			open(tryname)
			include_known_files[name] = tryname
			return tryname
		except IOError:
			pass
	return ""
	
includes = [(j, file) for j in get_includes(file)]
for current_include_name,last_include_path in includes:
	
	current_include_path = find_include_path(last_include_path, current_include_name)
	
	#print last_include_path + ": " + current_include_name + " -> " + current_include_path
	already_checked_files += [current_include_name]
	
	if current_include_path:
		newincludes = [ (j,current_include_path) for j in get_includes(current_include_path) if j not in already_checked_files]
		includes += newincludes
	else:
		sys.stderr.write( last_include_path+":0: error: Include not found "+current_include_name+"\n")
		sys.exit(1)

outincludes = [include_known_files[j[0]] for j in includes]
print os.path.basename(file.replace("cpp","o")) + ": " + file + " " + " ".join(outincludes)
sys.exit(0)