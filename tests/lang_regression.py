#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
from subprocess import *

os.environ['CF_DATADIR'] = "@CMAKE_SOURCE_DIR@/datafiles"

IMAGEDIR = "@CMAKE_SOURCE_DIR@/images/lang.diftest"
CUNEIFORM = "@CMAKE_BINARY_DIR@/cuneiform"
ACCURACY = "@CMAKE_BINARY_DIR@/cf_accuracy"

CMD = "%s --language %s --output %s --format textdebug --preserve-line-breaks '%s/%s.bmp'"
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
CUNEIFORM_VERSION = int(Popen([CUNEIFORM, '-V'], stdout=PIPE).communicate()[0].split()[-1])

tests_passed = 0
tests_failed = 0

def make_accuracy_report(orig, new):
    report_str = Popen([ACCURACY, orig, new], stdout=PIPE).communicate()[0]
    orig_name = os.path.split(orig)[1].split('.')[0] 
    report_file = open("%s.%d.acc" % (orig_name, CUNEIFORM_VERSION), 'w')
    report_file.write(report_str)
    
    lines = report_str.split('\n')
    print lines[3]
    print lines[4]

def compare_ocr_result(orig, new):
    retcode = call(['diff', '-bB', orig, new], stdout=PIPE)
    if(retcode != 0):
        global tests_failed
        tests_failed += 1
        orig_name = os.path.split(orig)[1].split('.')[0] 
        diff_output = open("%s.%d.diff" % (orig_name, CUNEIFORM_VERSION), 'w')
        call(['diff', '-bB', orig, new], stdout=diff_output)
        
    return retcode


for key, v in sorted(DATA.iteritems()):
    original_txt = "%s/%s.sample.txt" % (IMAGEDIR, v.lower())
    if(not os.path.exists(original_txt)):
        continue

#    print original_txt

    cmd = CMD % (CUNEIFORM, key, OUTPUT, IMAGEDIR, v.lower())
#    print cmd
    retcode = call(cmd, shell=True)
    if retcode != 0:
        print "%s failed" % v 
        tests_failed += 1
        print cmd
        continue

    if os.path.getsize(OUTPUT) == 0:
        print "%s failed. not output" % v
        sys.exit(1)
        
    retcode = compare_ocr_result(original_txt, OUTPUT)        
            
    if retcode == 0:
        print "%-15s Ok" % v
    else:
        print "%-15s Diff" % v
        make_accuracy_report(original_txt, OUTPUT)
        continue
        
    tests_passed += 1    
    
        
print "Tests passed: %d, failed: %d\n" % (tests_passed, tests_failed)

if(os.path.exists(OUTPUT)):
    os.unlink(OUTPUT)    
       
if tests_failed > 0:
    sys.exit(1)

