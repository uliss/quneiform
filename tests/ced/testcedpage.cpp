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

#include "testcedpage.h"
#include <ced/cedpage.h>
#include <ced/cedarchive.h>
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
    Size dpi(200,300);
    p.setImageDpi(dpi);
    p.setLanguage(LANGUAGE_ITALIAN);
    p.setUnrecognizedChar('!');
    Rect borders(Point(0, 0), 1000, 2000);
    p.setPageBorder(borders);
    p.addFont(FontEntry(1, 2, 3, "Times New Roman"));

    const char * fname = "serialize_cedpage.txt";

    // save data to archive
    {
        std::ofstream ofs(fname);
        CEDOutputArchive oa(ofs);
        // write class instance to archive
        oa << p;
    }

    CEDPage new_p;
    {
        // create and open an archive for input
        std::ifstream ifs(fname);
        assert(ifs);
        CEDInputArchive ia(ifs);
        // read class state from archive
        ia >> new_p;

        CPPUNIT_ASSERT_EQUAL(std::string("Test Image"), new_p.imageName());
        CPPUNIT_ASSERT_EQUAL(sz, new_p.imageSize());
        CPPUNIT_ASSERT_EQUAL(LANGUAGE_ITALIAN, new_p.language());
        CPPUNIT_ASSERT_EQUAL(dpi, new_p.imageDpi());
        CPPUNIT_ASSERT_EQUAL('!', new_p.unrecognizedChar());
        CPPUNIT_ASSERT_EQUAL(borders, new_p.pageBorder());
        CPPUNIT_ASSERT(new_p.fontCount() == 1);
    }

#endif
}
