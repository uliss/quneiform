#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import sys
import cf
import os

INPUT_FILE = 'indent.png'

FORMATS = (
     'text',
     'smarttext',
     'textdebug'
)

def removeOutput():
    fmtTest = cf.Tester('format')
    fmtTest.removeOutput(INPUT_FILE, FORMATS)

def test():
    fmtTest = cf.Tester('format')
    fmtTest.addArg('--append')

    for format in FORMATS:
        file_size = 0

        for i in range(0, 2):
            fmtTest.setFormat(format)
            fmtTest.cuneiformTest(fmtTest.makeFullImageName(INPUT_FILE))

            new_file_size = os.path.getsize(fmtTest.output())

            if new_file_size <= file_size:
                fmtTest.printError("Append failed")
                fmtTest.addFail()
            else:
                file_size = new_file_size

    fmtTest.removeArg('--append')

    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

if __name__ == '__main__':
    test()

