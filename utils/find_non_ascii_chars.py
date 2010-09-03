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
    new_filename = filename + '.new'
    new_f = open(new_filename, 'w')
    literals_num = 0
    try:
        for line in f:
            m = pattern.search(line)
            if m:
                repl = "'\\x%x' /* %s */" % (ord(m.group(1)), m.group(1))                
                new_line = pattern.sub(repl, line)
                new_f.write(new_line)
                literals_num += 1
            else:
                new_f.write(line)
    finally:
        f.close()


    if literals_num == 0:
        os.remove(new_filename)

    return False


if __name__ == "__main__":
    pattern = u"'([^\u0000-\u007E])'" 
    for f in get_sources():
        if not f:
            continue
        if find_string_in_file(re.compile(pattern), f):
	    pass            

