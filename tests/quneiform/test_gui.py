#!/usr/bin/env python
# -*- coding: utf-8 -*-

from subprocess import *

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

for file in "@CF_GUI_TESTS@".split(';'):
    name = "@CMAKE_BINARY_DIR@/test_" + file
    ret = call([name])
    if ret != 0:
        print "%sFailed%s" % (bcolor.FAIL, bcolor.END)
    else:
        print "%sOk%s" % (bcolor.OK, bcolor.END)

