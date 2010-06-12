#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import subprocess

os.environ['CF_DATADIR'] = "@CMAKE_SOURCE_DIR@/datafiles"
IMAGEDIR = "@CMAKE_SOURCE_DIR@/images/lang.diftest"
CUNEIFORM = "@CMAKE_BINARY_DIR@/cuneiform"

CMD = CUNEIFORM + " -l %s -o %s -f text '" + IMAGEDIR + "/%s.bmp'"
DATA = {'bul' : 'Bulgarian',
        'hrv' : 'Croatian',
        'cze' : 'Czech',
        'dan' : 'Danish',
        'dut' : 'Dutch',
        'eng' : 'English',
        'est' : 'Estonian',
        'fra' : 'French',
        'ger' : 'German',
        'hun' : 'Hungarian',
        'ita' : 'Italian',
        'lav' : 'Latvian',
        'lit' : 'Lithuanian',
        'pol' : 'Polish',
        'por' : 'Portuguese',
        'rum' : 'Romanian',
        'rus' : 'Russian',
        'srp' : 'Serbian',
        'slo' : 'Slovenian',
        'spa' : 'Spanish',
        'swe' : 'Swedish',
        'tur' : 'Turkish',
        'ukr' : 'Ukrainian'                 
         }
OUTPUT = "tmp.txt"
CMD += " 2>/dev/null"

tests_passed = 0
tests_failed = 0

for key, v in sorted(DATA.iteritems()):
    cmd = CMD % (key, OUTPUT, v.lower())
    # print cmd
    retcode = subprocess.call(cmd, shell=True)
    if retcode != 0:
        print "%s failed" % v
	tests_failed = tests_failed + 1
        print cmd
        continue
        
    tests_passed = tests_passed + 1

    if os.path.getsize(OUTPUT) == 0:
        print "%s failed. not output" % v
        sys.exit(1)
        
    if retcode == 0:
        print "%-15s Ok" % v
        
print "Tests passed: %d, failed: %d\n" % (tests_passed, tests_failed)

os.unlink(OUTPUT)    
       
if tests_failed > 0:
    sys.exit(1)

