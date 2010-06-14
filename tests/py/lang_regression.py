#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import cf

LANGS = {'bul' : 'Bulgarian',
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

def test():
    fmtTest = cf.Tester('lang.diftest')
    fmtTest.setFormat('textdebug')
    fmtTest.setSampleExt('txt')
    fmtTest.setLineBreaks(True)
    
    for key, lang in sorted(LANGS.iteritems()):
        fmtTest.setLanguage(key)
        img = fmtTest.makeFullImageName('%s.bmp' % lang.lower())
        if not fmtTest.diffTest(img):
            fmtTest.accuracy(img)
            
    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

if __name__ == '__main__':
    test()
