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

#include <fstream>
#include <sstream>

#include "testformatdetector.h"
#include "rdib/imageformatdetector.h"
#include "common/imageurl.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestFormatDetector);

#ifndef LOADER_TEST_IMAGE_DIR
#define LOADER_TEST_IMAGE_DIR "./"
#endif

using namespace cf;

void TestFormatDetector::testDetectByFilename()
{
#define ASSERT_EQ(format, fname) \
    CPPUNIT_ASSERT_EQUAL(FORMAT_##format, ImageFormatDetector::instance().detect(ImageURL(fname)))

    ASSERT_EQ(BMP, "test.bmp");
    ASSERT_EQ(BMP, "test.bMp");
    ASSERT_EQ(BMP, ".bmp");
    ASSERT_EQ(UNKNOWN, "bmp");
    ASSERT_EQ(UNKNOWN, "test.bmpbmp");
    ASSERT_EQ(UNKNOWN, "test.");
    ASSERT_EQ(UNKNOWN, "");
    ASSERT_EQ(GIF, "test.gif");
    ASSERT_EQ(JPEG, "test.jpg");
    ASSERT_EQ(JPEG, "test.JPEG");
    ASSERT_EQ(PNG, "test.png");
    ASSERT_EQ(PNM, "test.ppm");
    ASSERT_EQ(PNM, "test.pgm");
    ASSERT_EQ(PNM, "test.pbm");
    ASSERT_EQ(PNM, "test.pnm");
    ASSERT_EQ(XPM, "test.xpm");
    ASSERT_EQ(TIFF, "test.tiff");
    ASSERT_EQ(TIFF, "test.tif");
}

void TestFormatDetector::testDetectByContent() {
    std::stringstream stream;
    stream << "GIFsssstegggggggggg";
    CPPUNIT_ASSERT_EQUAL(cf::FORMAT_GIF, ImageFormatDetector::instance().detect(stream));
    std::ifstream bad_stream;
    CPPUNIT_ASSERT_EQUAL(cf::FORMAT_UNKNOWN, ImageFormatDetector::instance().detect(bad_stream));
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
