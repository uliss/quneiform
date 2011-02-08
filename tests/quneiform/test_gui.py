#!/usr/bin/env python
# -*- coding: utf-8 -*-

from subprocess import *
import sys

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

failed = 0
success = 0

for file in "@CF_GUI_TESTS@".split(';'):
    name = "@CMAKE_BINARY_DIR@/test_" + file
    ret = call([name])
    if ret != 0:
        print "%sFailed%s" % (bcolor.FAIL, bcolor.END)
        failed += 1
    else:
        print "%sOk%s" % (bcolor.OK, bcolor.END)
        success += 1

if failed > 0:
    print "Summary: %sFAIL%s - %i tests failed" % (bcolor.FAIL, bcolor.END, failed)
    sys.exit(1)
else:
    print "Summary: %sOK%s - all tests passed" % (bcolor.OK, bcolor.END)
    sys.exit(0)

