#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import sys
import cf

INPUT_FILES = (
    'columns.png',
)

FORMATS = (
     'html',
     'text',
     'smarttext',
     'native',
     'native-xml',
     'djvutxt',
     'djvuxml',
     'odf',
     'fb2'
)

def test():
    fmtTest = cf.Tester('format')

    for img in INPUT_FILES:
        for format in FORMATS:
            fmtTest.setFormat(format)
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
