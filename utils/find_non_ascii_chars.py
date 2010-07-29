#!/usr/bin/env python

import os
import re
import sys


def get_sources():
    res = os.popen("find %s -name '*.h' -o -name '*.cpp'" % "../src").read()
    return res.split("\n")


def find_string_in_file(pattern, filename):
    #print "Looking in ", filename,
    f = open(filename, "r")
    try:
        for line in f:
            m = pattern.search(line)
            if m:
                #print line,
                print "%x" % ord(m.group(1))                
                print filename
                f.close()
                return True
    finally:
        f.close()
    return False


if __name__ == "__main__":
    pattern = u"'([^\u0000-\u007E])'" 
    for f in get_sources():
        if not f:
            continue
        if find_string_in_file(re.compile(pattern), f):
	    pass            

