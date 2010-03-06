#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import subprocess

CMD = "./cuneiform -l ger -o tmp.%s -f %s '../../cifocr/images/lang.diftest/german.bmp'"
DATA = ['text', 'textdebug', 'smarttext', 'html', 'hocr', 'rtf', 'native']

CMD += " 2>/dev/null"

tests_passed = 0
tests_failed = 0

for key in DATA:
    cmd = CMD % (key, key)
    #print cmd
    retcode = subprocess.call(cmd, shell=True)
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

