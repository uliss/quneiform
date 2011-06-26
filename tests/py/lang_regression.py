#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import cf

LANGS = {'bel': 'Belarusian',
        'bul' : 'Bulgarian',
        'hrv' : 'Croatian',
        'cze' : 'Czech',
        'dan' : 'Danish',
        'dut' : 'Dutch',
        'eng' : 'English',
        'est' : 'Estonian',
        'fra' : 'French',
        'ger' : 'German',
        'hun' : 'Hungarian',
        'ita' : 'Italian',
        'lav' : 'Latvian',
        'lit' : 'Lithuanian',
        'pol' : 'Polish',
        'por' : 'Portuguese',
        'rum' : 'Romanian',
        'rus' : 'Russian',
        'srp' : 'Serbian',
        'slo' : 'Slovenian',
        'spa' : 'Spanish',
        'swe' : 'Swedish',
        'tur' : 'Turkish',
        'ukr' : 'Ukrainian'                 
         }

FORMATS = {
    'textdebug' : 'txt',
    'html' : 'html',
    'hocr' : 'hocr'
    }

def test_format(format, ext):
    fmtTest = cf.Tester('lang.diftest')
    fmtTest.setFormat(format)
    fmtTest.setSampleExt(ext)
    fmtTest.setLineBreaks(True)
    fmtTest.addArg('--show-alternatives')

    for key, lang in sorted(LANGS.iteritems()):
        fmtTest.setLanguage(key)
        extension = 'bmp'
        if key == 'bel':
            extension = 'png'

        arg1 = '--output-image-dir'
        arg2 = '%s.sample_files' % (lang.lower())
        fmtTest.addArg(arg1)
        fmtTest.addArg(arg2)

        img = fmtTest.makeFullImageName('%s.%s' % (lang.lower(), extension))

        if not fmtTest.diffTest(img) and format == 'textdebug':
            fmtTest.accuracy(img)

        fmtTest.removeArg(arg1)
        fmtTest.removeArg(arg2)

    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

def test():
    args = sys.argv[1:]

    if len(args) == 0:
        for format, extension in FORMATS.iteritems():
            test_format(format, extension)
    else:
        for format in args:
            if not FORMATS.has_key(format):
                print "unknown output format: %s" %(format)
                break

            test_format(format, FORMATS[format])

if __name__ == '__main__':
    test()
