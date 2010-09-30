#!/usr/bin/python

import os, sys, re, shutil

def get_sources():
    res = os.popen("find %s -name '*.h' -o -name '*.c' -o -name '*.cpp'" % "../src").read()
    return res.split("\n")


def find_string_in_file(pattern, filename):
    #print "Looking in ", filename,
    f = open(filename, "r")
    try:
        for line in f:
	    if pattern.search(line):
	        #print line
		f.close()
		return True
    finally:
        f.close()
    return False

def process_file(pattern, to, filename):
    src = open(filename, "r")
    tmp = os.path.basename(filename) + ".tmp"
    dest = open(tmp, "w")
    try:
        for line in src:
	    s, n = pattern.subn(to, line)
	    dest.write(s)
	    if n > 0:
	        print "-", line, "+", s
    finally:
        src.close()
	dest.close()
	shutil.copy(filename, os.path.basename(filename) + ".bak")
	shutil.move(tmp, filename)
	
if __name__ == "__main__":
    if  len(sys.argv) != 3:
        print "Usage replace.py FROM TO"
	sys.exit(1)

    if sys.argv[1] == sys.argv[2]:
        print "Error: patterns are equal"
	sys.exit(2)

    print "Replace %s => %s" % (sys.argv[1], sys.argv[2])
    pattern = r"(^|[^a-zA-Z_0-9])%s([^a-zA-Z_0-9])" % sys.argv[1]
    pattern_dest = r'\1%s\2' % sys.argv[2] 
    for f in get_sources():
        if not f:
	    continue
        if find_string_in_file(re.compile(pattern), f):
	    print "Replace %s to %s in %s" %(pattern, pattern_dest, os.path.basename(f))
	    process_file(re.compile(pattern),  pattern_dest, f)
