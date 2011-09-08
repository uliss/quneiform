#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import os

os.chdir("@CMAKE_BINARY_DIR@/py/")
 
def header(msg):
    print 
    print msg
    print "============================="

header("Quick process test")
import quick_process
quick_process.test()

header("Append test")
import append
append.test()
    
header("Align test")
import align
align.test()

header("Font styles test")
import font_styles
font_styles.test()

header("Indent test")
import indent
indent.test()

header("Hyphens test")
import hyphens
hyphens.test()

header("Columns test")
import columns
columns.test()

header("Format quick test")
import format_quick
format_quick.test()

header("Language quick test")
import lang_quick
lang_quick.test()

header("Language regression test")
import lang_regression
lang_regression.test()

header("Image export test")
import image_export
image_export.test()

header("Image formats test")
import image_formats
image_formats.test()

#header("Summary:")
#print "Test total: %d, passed: %d, failed: %d" % ()
