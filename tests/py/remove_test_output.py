#!@PYTHON_EXECUTABLE@
# -*- coding: utf-8 -*-

import os

os.chdir("@CMAKE_BINARY_DIR@/py/")

import align
align.removeOutput()
import append
append.removeOutput()
import columns
columns.removeOutput()
import lang_quick
lang_quick.removeOutput()
import lang_regression
lang_regression.removeOutput()
