#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import sys
import cf
import os
import glob

INPUT_FILES = (
    'multipage.tif',
    'english.png'
)

FORMATS = (
     ('text')
)

def removeOutput():
    fmtTest = cf.Tester('quneiform')
    fmtTest.removeOutput(INPUT_FILES, ('text'))

def test():
    fmtTest = cf.Tester('quneiform')
    fmtTest.setFormat('text')

    fmtTest.diffTestContent(fmtTest.makeFullImageName('multipage.tif'), 'ENGLISH')
    fmtTest.setPageNumber(1)
    fmtTest.diffTestContent(fmtTest.makeFullImageName('multipage.tif'), 'GERMAN')

    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

if __name__ == '__main__':
    test()
