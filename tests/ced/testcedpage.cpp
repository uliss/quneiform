/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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
#include "testcedpage.h"
#include "../test_common.h"
#include <ced/cedpage.h>
#include <ced/cedsection.h>
#include <common/tostring.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDPage);

using namespace CIF;

#define CF_SERIALIZE

void TestCEDPage::testSerialize() {
#ifdef CF_SERIALIZE
    CEDPage p;
    p.setImageName("Test Image");
    Size sz(10, 20);
    p.setImageSize(sz);
    Size dpi(200, 300);
    p.setImageDpi(dpi);
    p.setLanguage(LANGUAGE_ITALIAN);
    Rect borders(Point(0, 0), 1000, 2000);
    p.setPageBorder(borders);

    const char * fname = "serialize_cedpage.txt";

    writeToTextArchive(fname, p);

    CEDPage new_p;
    readFromTextArchive(fname, new_p);

    CPPUNIT_ASSERT_EQUAL(std::string("Test Image"), new_p.imageName());
    CPPUNIT_ASSERT_EQUAL(sz, new_p.imageSize());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_ITALIAN, new_p.language());
    CPPUNIT_ASSERT_EQUAL(dpi, new_p.imageDpi());
    CPPUNIT_ASSERT_EQUAL(borders, new_p.pageBorder());
#endif
}

void TestCEDPage::testSerializeXml() {
#ifdef CF_SERIALIZE
    CEDPage p;
    p.setImageName("Test Image");
    Size sz(10, 20);
    p.setImageSize(sz);
    Size dpi(200, 300);
    p.setImageDpi(dpi);
    p.setLanguage(LANGUAGE_ITALIAN);
    Rect borders(Point(0, 0), 1000, 2000);
    p.setPageBorder(borders);

    const char * fname = "serialize_cedpage.xml";

    writeToXmlArchive(fname, "cedpage", p);

    CEDPage new_p;
    readFromXmlArchive(fname, "cedpage", new_p);

    CPPUNIT_ASSERT_EQUAL(std::string("Test Image"), new_p.imageName());
    CPPUNIT_ASSERT_EQUAL(sz, new_p.imageSize());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_ITALIAN, new_p.language());
    CPPUNIT_ASSERT_EQUAL(dpi, new_p.imageDpi());
    CPPUNIT_ASSERT_EQUAL(borders, new_p.pageBorder());
#endif
}
