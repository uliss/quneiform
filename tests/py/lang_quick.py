#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import sys
import cf

LANGS = {'bel' : 'Belarusian',
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

def test():
    fmtTest = cf.Tester('lang.diftest')
    fmtTest.setFormat('text')
    
    for key, lang in sorted(LANGS.iteritems()):
        fmtTest.setLanguage(key)
        extension = 'bmp'
        if key == 'bel':
            extension = 'png'

        fmtTest.cuneiformTest(fmtTest.makeFullImageName('%s.%s' % (lang.lower(), extension)))
            
    if fmtTest.passed():
        return True
    else:
        if __name__ == '__main__':
            sys.exit(1)
        else:
            return False

if __name__ == '__main__':
    test()
