#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import cf

INPUT_FILES = (
    'indent.png',
)

def test():
    fmtTest = cf.Tester('format')
    fmtTest.setFormat('html')
    
    for img in INPUT_FILES:
        fmtTest.diffTest(fmtTest.makeFullImageName(img))
        
    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

if __name__ == '__main__':
    test()

