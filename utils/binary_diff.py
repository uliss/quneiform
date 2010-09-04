#!/usr/bin/env python

import sys, os 
import cuneiform_md5

md5_file = "@CMAKE_BINARY_DIR@/cuneiform.md5"
count = 0

class bcolor:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OK = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    END = '\033[0m'

    def disable(self):
        self.HEADER = ''
        self.OKBLUE = ''
        self.OK = ''
        self.WARNING = ''
        self.FAIL = ''
        self.END = ''


if os.path.exists(md5_file):
    f = open(md5_file, 'r')
    for line in f.readlines():
        pair = line.split(' ') 
        if len(pair) != 2:
            continue
   
        new_hash = cuneiform_md5.md5sum(pair[1].rstrip())    
        name = os.path.basename(pair[1].rstrip())
        if pair[0] != new_hash.keys()[0]:
            print "%s%-30s Diff%s" % (bcolor.FAIL, name, bcolor.END)
            count += 1
        else:
            print "%-30s %sOk%s" % (name, bcolor.OK, bcolor.END) 
    
    if count > 0:
        print "\n%sWarning!%s %d binary files changed" % (bcolor.FAIL, bcolor.END, count)
    else:
        print "%sNo binary diff%s" % (bcolor.OK, bcolor.END)

else:
    hash = cuneiform_md5.generate()
    f = open(md5_file, 'w')
    for key, value in hash.iteritems():
        f.write("%s %s\n" % (key, value)) 



