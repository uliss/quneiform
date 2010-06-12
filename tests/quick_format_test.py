#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
from subprocess import *

os.environ['CF_DATADIR'] = "@CMAKE_SOURCE_DIR@/datafiles"

IMAGE = "@CMAKE_SOURCE_DIR@/images/lang.diftest/german.bmp"
CUNEIFORM = "@CMAKE_BINARY_DIR@/cuneiform"

CMD = CUNEIFORM + " -l ger -o tmp.%s -f %s '" + IMAGE + "'"
DATA = ['text', 'textdebug', 'smarttext', 'html', 'hocr', 'odf', 'rtf', 'native']

tests_passed = 0
tests_failed = 0

for key in DATA:
    cmd = CMD % (key, key)
    #print cmd
    retcode = call(cmd, stdout=PIPE, stderr=PIPE, shell=True)
    if retcode > 0:
        print "%s failed" % key
	tests_failed = tests_failed + 1
        print cmd
        continue
        #sys.exit(1)
    elif retcode < 0:
        sys.exit(1)
        
    tests_passed = tests_passed + 1

    if os.path.getsize("tmp." + key) == 0:
        print "%s failed. not output" % v
        sys.exit(1)
        
    if retcode == 0:
        print "%-15s Ok" % key
        
print "Tests passed: %d, failed: %d\n" % (tests_passed, tests_failed)

#os.unlink(OUTPUT)    
       
if tests_failed > 0:
    sys.exit(1)

