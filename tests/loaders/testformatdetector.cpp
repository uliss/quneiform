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
CPPUNIT_TEST_SUITE_REGISTRATION(TestFormatDetector);

using namespace CIF;

void TestFormatDetector::testInit()
{
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_BMP, ImageFormatDetector::instance().detect("test.bmp"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_BMP, ImageFormatDetector::instance().detect("test.BMP"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_BMP, ImageFormatDetector::instance().detect("test.bMp"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_BMP, ImageFormatDetector::instance().detect(".bmp"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_UNKNOWN, ImageFormatDetector::instance().detect("bmp"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_UNKNOWN, ImageFormatDetector::instance().detect("test.bmpbmp"));
    CPPUNIT_ASSERT_EQUAL(CIF::FORMAT_UNKNOWN, ImageFormatDetector::instance().detect("test."));
}
