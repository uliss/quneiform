#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
from subprocess import *

os.environ['CF_DATADIR'] = "@CMAKE_SOURCE_DIR@/datafiles"

# globals exe's and paths
CUNEIFORM = "@CMAKE_BINARY_DIR@/cuneiform"
ACCSUM = "@CMAKE_BINARY_DIR@/cf_accsum"
ACCURACY = "@CMAKE_BINARY_DIR@/cf_accuracy"
IMAGEDIR = "@CMAKE_SOURCE_DIR@/images"
DIFFOPTS = '-bB'

class Tester:
    _imagedir = ''
    _images = []
    _version = None
    _tests_passed = 0
    _tests_failed = 0
    _language = None
    _output = None
    _output_image_dir = None
    _format = None
    _line_breaks = False
    _sample_ext = None
    
    def __init__(self, imagedir=''):
        self._imagedir = os.path.join(IMAGEDIR, imagedir)
        
    def __del__(self):
        self.printTestStat()
        
    def accuracy(self, img):
        output = self.makeOutput(img)
        report_str = Popen([ACCURACY, self.makeSampleName(img), output], stdout=PIPE).communicate()[0]
        
        report_file = open("%s.acc" % output, 'w')
        report_file.write(report_str)
        
        lines = report_str.split('\n')
        if len(lines) > 4:
            print lines[3]  
            print lines[4]
    
    def addImage(self, img):
        self._images.append(img)
        
    ''' adds image to tester '''
    def addImages(self, files):
        self._images += files    
      
    ''' removes all images from tester '''  
    def clear(self):
        self._images = []
        
    def cuneiform(self, args, **kwargs):
        cmd = [CUNEIFORM] + args
        retcode = call(cmd, **kwargs)
        if retcode != 0:
            print ' '.join(cmd)
        return retcode
    
    def cuneiformTest(self, img):
        if not os.path.exists(img):
            sys.stderr.write("image file not exists: %s\n" % img)
            return False
            
        if self._format is None:
            print 'Error: output file not specified and output format is not set'
            return  False
            
        self._output = self.makeOutput(img)
        
        retcode = self.cuneiform(self.makeArgs(img), stdout=PIPE, stderr=PIPE)
        if retcode != 0:
            print "%-35s %-15s FAIL" % (os.path.basename(img), "OCR")
            self._tests_failed += 1
            return False
            
        if os.path.getsize(self._output) == 0:
            print "%-35s %-15s FAIL. No output" % (os.path.basename(img), "OCR")
            self._tests_failed += 1
            return False
        else:
            print "%-35s %-15s Ok" % (os.path.basename(img), 'OCR(%s)' % self._format)
            self._tests_passed += 1
        
        return True
    
    def diff(self, first, second, **kwargs):
        cmd = ['diff', DIFFOPTS, first, second]
        #print cmd
        return call(cmd, **kwargs)
    
    def diffTest(self, img):
        if not self.cuneiformTest(img):
            return False
        
        sample_name = self.makeSampleName(img)
        if not os.path.exists(sample_name):
            print "%-35s Diff failed. Sample output not exists: %s" % (os.path.basename(img), sample_name)
            self._tests_failed += 1
            return False
        
        diff_name = self.makeDiffName()
        diff_output = open(diff_name, 'w')
        retcode = self.diff(sample_name, self._output, stdout=diff_output)
        if retcode != 0:
            print "%-35s Difference found" % os.path.basename(img)
            self._tests_failed += 1
            return False
        else:
            self._tests_passed += 1
            os.unlink(self._output)
            os.unlink(diff_name)
            return True
            
    def makeArgs(self, img):
        args = []
        if self._language is not None:
            args += ['--language', self._language]
        if self._output is not None:
            args += ['--output', self._output]
        if self._format is not None:
            args += ['--format', self._format]
        if self._output_image_dir is not None:
            args += ['--output-image-dir', self._output_image_dir]
            
        if self._line_breaks:
            args.append('--preserve-line-breaks')
            
        args.append(img)
            
        return args
    
    def makeDiffName(self):
        return "%s.diff" % self._output
    
    def makeFullImageName(self, image):
        return os.path.join(IMAGEDIR, self._imagedir, image)
    
    def makeOutput(self, image):
        return '%s.%d.%s' % (os.path.splitext(os.path.basename(image))[0], self.version(), self._format)
    
    def makeSampleName(self, img):
        path = os.path.split(img)
        name = os.path.join(path[0], "%s.sample." % os.path.splitext(path[1])[0]) 
        if self._sample_ext is not None:
            name += self._sample_ext
        else:
            name += self._format
        
        return name
    
    def passed(self):
        return self._tests_failed == 0
    
    def printTestStat(self):
        print "Tests total: %d, passed: %d, failed: %d" % (self.total(), self._tests_passed, self._tests_failed)
            
    def runDiffTest(self):
        for img in self._images:
            self.diffTest(img)
            
    def setFormat(self, format):
        self._format = format
        
    def setImageOutputDir(self, path):
        self._output_image_dir = path
            
    def setLanguage(self, lang):
        self._language = lang
        
    def setLineBreaks(self, value):
        if value:
            self._line_breaks = True
        else:
            self._line_breaks = False
            
    def setSampleExt(self, ext):
        self._sample_ext = ext
        
    def total(self):
        return self._tests_failed + self._tests_passed
                
    ''' returns cuneiform version ''' 
    def version(self):
        if self._version is None:
            self._version = int(Popen([CUNEIFORM, '-V'], stdout=PIPE).communicate()[0].split()[-1])
        
        return self._version

