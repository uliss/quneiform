/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
 *   serge.poltavski@gmail.com                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program. If not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/
#include "testformatdetector.h"
#include <rdib/imageformatdetector.h>
#include <fstream>
#include <sstream>
CPPUNIT_TEST_SUITE_REGISTRATION(TestFormatDetector);

#ifndef LOADER_TEST_IMAGE_DIR
#define LOADER_TEST_IMAGE_DIR "./"
#endif

using namespace CIF;

void TestFormatDetector::testDetectByFilename()
{
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_BMP, ImageFormatDetector::instance().detect("test.bmp"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_BMP, ImageFormatDetector::instance().detect("test.BMP"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_BMP, ImageFormatDetector::instance().detect("test.bMp"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_BMP, ImageFormatDetector::instance().detect(".bmp"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_UNKNOWN, ImageFormatDetector::instance().detect("bmp"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_UNKNOWN, ImageFormatDetector::instance().detect("test.bmpbmp"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_UNKNOWN, ImageFormatDetector::instance().detect("test."));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_UNKNOWN, ImageFormatDetector::instance().detect(""));

    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_GIF, ImageFormatDetector::instance().detect("test.gif"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_JPEG, ImageFormatDetector::instance().detect("test.jpg"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_JPEG, ImageFormatDetector::instance().detect("test.JPEG"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_PNG, ImageFormatDetector::instance().detect("test.png"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_PNM, ImageFormatDetector::instance().detect("test.ppm"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_PNM, ImageFormatDetector::instance().detect("test.pbm"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_PNM, ImageFormatDetector::instance().detect("test.pgm"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_PNM, ImageFormatDetector::instance().detect("test.pnm"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_XPM, ImageFormatDetector::instance().detect("test.xpm"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_TIFF, ImageFormatDetector::instance().detect("test.tiff"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_TIFF, ImageFormatDetector::instance().detect("test.tif"));
}

void TestFormatDetector::testDetectByContent()
{
    std::stringstream stream;
    stream << "GIFsssstegggggggggg";
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_GIF, ImageFormatDetector::instance().detect(stream));
    std::ifstream bad_stream;
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_UNKNOWN, ImageFormatDetector::instance().detect(bad_stream));
    std::vector<std::string> extensions = ImageFormatDetector::instance().knownExtensions();
    for (size_t i = 0; i < extensions.size(); i++) {
        std::string name = LOADER_TEST_IMAGE_DIR + std::string("test.") + extensions[i];
        std::ifstream is(name.c_str());
        if (!is) {
            std::cerr << "Error opening file: " << name << std::endl;
            continue;
        }
        image_format_t fmt = ImageFormatDetector::instance().detect(is);
        CPPUNIT_ASSERT(fmt != FORMAT_UNKNOWN);
    }

}
