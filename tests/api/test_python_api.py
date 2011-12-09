#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import imp

cf = imp.load_source('pyneiform', '@EXECUTABLE_OUTPUT_PATH@/api/swig/python/pyneiform.py')

print "version:", cf.VERSION
print "build:  ", cf.BUILD_NUMBER

# Format options
fopts = cf.FormatOptions();

fopts.setLanguage(cf.LANGUAGE_ENGLISH)
assert fopts.language() == cf.LANGUAGE_ENGLISH

fopts.setWriteBom(0)
assert fopts.writeBom() == 0

# Recognition options
ropts = cf.RecognitionOptions();

ropts.setLanguage(cf.LANGUAGE_ENGLISH)
assert ropts.language() == cf.LANGUAGE_ENGLISH

ropts.setDotMatrix(1)
assert ropts.dotMatrix() == 1

ropts.setFax(1)
assert ropts.fax() == 1

ropts.setSearchPictures(1)
assert ropts.searchPictures() == 1

ropts.setSingleColumn(1)
assert ropts.singleColumn() == 1

ropts.setSpellCorrection(1)
assert ropts.spellCorrection() == 1

page = cf.recognize("@CMAKE_SOURCE_DIR@/images/quneiform/english.png", ropts, fopts)

text = page.toString(cf.FORMAT_TEXT, fopts).strip();
print text
assert text == "ENGLISH"
