#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import cf

FORMATS = (
     'html',
     'text',
     'smarttext',
     'native',
     'odf'
)

def test():
    fmtTest = cf.Tester('format')
    
    for format in FORMATS:
       fmtTest.setFormat(format)
       fmtTest.diffTest(fmtTest.makeFullImageName('hyphen.png'))
     
    fmtTest.setLineBreaks(True)
      
    for format in FORMATS:
       fmtTest.setFormat(format)
       fmtTest.diffTest(fmtTest.makeFullImageName('hyphen_line_break.png'))
        
    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

if __name__ == '__main__':
    test()

