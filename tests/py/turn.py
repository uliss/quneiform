#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import sys
import cf
import os
import glob

INPUT_FILES = (
    'english_rotated_90.png',
    'english_rotated_180.png',
    'english_rotated_270.png',
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

    fmtTest.setTurn(0)
    fmtTest.diffTest(fmtTest.makeFullImageName('english.png'))
    fmtTest.setTurn(360)
    fmtTest.diffTest(fmtTest.makeFullImageName('english.png'))

    fmtTest.setTurn(-90)
    fmtTest.diffTest(fmtTest.makeFullImageName('english_rotated_90.png'))
    fmtTest.setTurn(270)
    fmtTest.diffTest(fmtTest.makeFullImageName('english_rotated_90.png'))

    fmtTest.setTurn(180)
    fmtTest.diffTest(fmtTest.makeFullImageName('english_rotated_180.png'))
    fmtTest.setTurn(-180)
    fmtTest.diffTest(fmtTest.makeFullImageName('english_rotated_180.png'))

    fmtTest.setTurn(-270)
    fmtTest.diffTest(fmtTest.makeFullImageName('english_rotated_270.png'))
    fmtTest.setTurn(90)
    fmtTest.diffTest(fmtTest.makeFullImageName('english_rotated_270.png'))

    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

if __name__ == '__main__':
    test()

