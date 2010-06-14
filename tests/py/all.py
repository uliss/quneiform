#!/usr/bin/env python 
# -*- coding: utf-8 -*-

import os

os.chdir("@CMAKE_BINARY_DIR@/py/")
 
def header(msg):
    print msg
    print "============================="
    
header("Align test")
import align

header("Format test")
import format_quick

header("Language quick test")
import lang_quick

header("Language regression test")
import lang_regression