#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import imp

cf = imp.load_source('pyneiform', '@EXECUTABLE_OUTPUT_PATH@/api/swig/python/pyneiform.py')

print "Ok"
