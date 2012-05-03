#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import sys
import cf
import os
import glob

def removeOutput():
    fmtTest = cf.Tester('format')
    fmtTest.removeOutput('area', 'text')

def test():
    fmtTest = cf.Tester('format')
    fmtTest.setFormat('text')

    fmtTest.setFormat(format)
    fmtTest.diffTest(fmtTest.makeFullImageName('area.png'))

    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

if __name__ == '__main__':
    test()
