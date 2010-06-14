#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import cf

INPUT_FILES = (
    'align_center.png',
    'align_justify.png',
    'align_left.png',
    'align_right.png'
)

fmtTest = cf.Tester('format')
fmtTest.setFormat('html')

for img in INPUT_FILES:
    fmtTest.diffTest(fmtTest.makeFullImageName(img))
        
if not fmtTest.passed():
    sys.exit(1)
