#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os, sys
import zipfile
from subprocess import *

SCRIPT_DIR = "@CMAKE_SOURCE_DIR@/tests/py"

class OdfReader:
    def __init__(self,filename):
        """
        Open an ODF file.
        """
        self.filename = filename
        self.m_odf = zipfile.ZipFile(filename)
        self.filelist = self.m_odf.infolist()

    def getContents(self):
        return self.m_odf.read('content.xml')
    
def test():
    cmd = ['python', SCRIPT_DIR + '/lpod-show.py', '-h']
    return call(cmd)
               
if __name__ == '__main__':
    test()
