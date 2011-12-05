#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import imp

cf = imp.load_source('pyneiform', '@EXECUTABLE_OUTPUT_PATH@/api/swig/python/pyneiform.py')

print "version:", cf.VERSION
print "build:  ", cf.BUILD_NUMBER

fopts = cf.FormatOptions();
ropts = cf.RecognitionOptions();

fopts.setLanguage(cf.LANGUAGE_ENGLISH)
ropts.setLanguage(cf.LANGUAGE_ENGLISH)

page = cf.recognize("@CMAKE_SOURCE_DIR@/images/quneiform/english.png", ropts, fopts)
page.save("/dev/stdout", cf.FORMAT_TEXT, fopts)
