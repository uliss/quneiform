#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
from subprocess import *
from string import join

os.environ['CF_DATADIR'] = "@CMAKE_SOURCE_DIR@/datafiles"

IMAGEDIR = "@CMAKE_SOURCE_DIR@/images/format"
CUNEIFORM = "@CMAKE_BINARY_DIR@/cuneiform"
CUNEIFORM_VERSION = int(Popen([CUNEIFORM, '-V'], stdout=PIPE).communicate()[0].split()[-1])

CMD = [CUNEIFORM, "--format", "html"]
INPUT_FILES = (
    'align_center',
    'align_justify',
    'align_left',
    'align_right'
)


tests_passed = 0
tests_failed = 0

def compare_ocr_result(orig, new):
    retcode = call(['diff', '-bB', orig, new], stdout=PIPE)
    if(retcode != 0):
        global tests_failed
        tests_failed += 1
        orig_name = os.path.split(orig)[1].split('.')[0] 
        diff_output = open("%s.%d.diff" % (orig_name, CUNEIFORM_VERSION), 'w')
        call(['diff', '-bB', orig, new], stdout=diff_output)
        
    return retcode


for fname in INPUT_FILES:
    html = "%s/%s.sample.html" % (IMAGEDIR, fname)
    img = "%s/%s.png" % (IMAGEDIR, fname)
    if(not os.path.exists(img) or not os.path.exists(html)):
        continue

    output_html = '%s.%d.html' % (fname, CUNEIFORM_VERSION)

    cmd = CMD + ['--output', output_html, img]
    
    retcode = call(cmd, stderr=PIPE)
    if retcode != 0:
        print "%s failed" % fname 
        tests_failed += 1
        print ' '.join(cmd)
        continue

    if os.path.getsize(output_html) == 0:
        print "%s failed. not output" % v
        sys.exit(1)
        
    retcode = compare_ocr_result(html, output_html)        
            
    if retcode == 0:
        print "%-15s Ok" % fname
    else:
        print "%-15s Diff" % fname
        continue
        
    tests_passed += 1    
    
        
print "Tests passed: %d, failed: %d\n" % (tests_passed, tests_failed)   
       
if tests_failed > 0:
    sys.exit(1)

