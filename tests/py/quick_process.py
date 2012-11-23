#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import sys
import os
import cf

FORMATS = ('text', 'html')

def test():
    fmtTest = cf.Tester('lang.diftest')
    # fix for search cuneiform-worker in compiled tree, not in  installed directory
    # to avoid worker version mismatch
    os.chdir("..")

    for key in FORMATS:
        fmtTest.setFormat(key)
        fmtTest.cuneiformTest(fmtTest.makeFullImageName('german.bmp'), True)

    os.chdir("py")

    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

if __name__ == "__main__":
    test()
