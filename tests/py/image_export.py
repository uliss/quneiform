#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
import cf

INPUT_FILES = ( 
    'image_centered.png',
)

def test():
    fmtTest = cf.Tester('format')
    fmtTest.setFormat('html')
    fmtTest.setImageOutputDir('image_centered_files')
    fmtTest.addArg('--show-alternatives')
    
    for img in INPUT_FILES:
        fmtTest.diffTest(fmtTest.makeFullImageName(img))
        if not os.path.exists(os.path.join(os.path.splitext(img)[0], 'image_0.png')):
            return False
        
    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

if __name__ == '__main__':
    test()

