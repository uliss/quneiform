#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import subprocess

CMD = "./cuneiform -l %s -o %s -f text '../images/lang.diftest/%s.bmp'"
DATA = {'bul' : 'Bulgarian',
        'hrv' : 'Croatian',
        'cze' : 'Czech',
        'dan' : 'Danish',
        'dut' : 'Dutch',
        'eng' : 'English2cl',
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
        'slo' : 'Slovenian',
        'spa' : 'Spanish',
        'swe' : 'Swedish',
        'ukr' : 'Ukrainian'                 
         }
OUTPUT = "tmp.txt"
CMD += " 2>/dev/null"

os.environ['CF_DATADIR'] = "../datafiles"

for key, v in sorted(DATA.iteritems()):
    cmd = CMD % (key, OUTPUT, v.lower())
    # print cmd
    retcode = subprocess.call(cmd, shell=True)
    if retcode > 0:
        print "%s failed" % v
        sys.exit(1)
    elif retcode < 0:
        sys.exit(1)
        
    if os.path.getsize(OUTPUT) == 0:
        print "%s failed. not output" % v
        sys.exit(1)
        
    if retcode == 0:
        print "%-15s Ok" % v
        
os.unlink(OUTPUT)    
       
    

