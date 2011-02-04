#!/usr/bin/env python
# -*- coding: utf-8 -*-

from subprocess import *

for file in "@CF_GUI_TESTS@".split(';'):
    name = "@CMAKE_BINARY_DIR@/test_" + file
    call([name])
