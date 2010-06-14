#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import cf

FORMATS = ('text', 'textdebug', 'smarttext', 'html', 'hocr', 'odf', 'rtf', 'native')

fmtTest = cf.Tester('lang.diftest')

for key in FORMATS:
    fmtTest.setFormat(key)
    fmtTest.cuneiformTest(fmtTest.makeFullImageName('german.bmp'))
        
if not fmtTest.passed():
    sys.exit(1)

