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
#include "testcolor.h"
#include "../test_common.h"
#include <common/color.h>
#include <common/tostring.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestColor);
using namespace CIF;

void TestColor::testSerialize() {
#ifdef CF_SERIALIZE
    const Color c(1, 2, 3);
    const Color n = Color::null();

    const char * TEXT_OUT = "serialize_color.txt";

    // save data to archive
    {
        std::ofstream ofs(TEXT_OUT);
        CEDOutputArchive oa(ofs);
        // write class instance to archive
        oa << c << n;
    }

    Color new_c;
    CPPUNIT_ASSERT(c != new_c);
    {
        // create and open an archive for input
        std::ifstream ifs(TEXT_OUT);
        CEDInputArchive ia(ifs);
        // read class state from archive
        ia >> new_c;

        CPPUNIT_ASSERT_EQUAL(c, new_c);

        ia >> new_c;
        CPPUNIT_ASSERT_EQUAL(n, new_c);
    }
#endif
}

void TestColor::testSerializeXml() {
#ifdef CF_SERIALIZE
    const Color c(10, 20, 30);
    const Color n = Color::null();

    const char * XML_OUT = "serialize_color.xml";

    // save data to archive
    {
        std::ofstream ofsx(XML_OUT);
        CEDXmlOutputArchive oax(ofsx);
        // write class instance to archive
        writeToXml(oax, "color", c);
        writeToXml(oax, "color", n);
    }

    {
        Color new_c;
        CPPUNIT_ASSERT(c != new_c);

        // create and open an archive for input
        std::ifstream ifs(XML_OUT);
        CEDXmlInputArchive ia(ifs);

        readFromXml(ia, "color", new_c);
        CPPUNIT_ASSERT_EQUAL(c, new_c);
        readFromXml(ia, "color", new_c);
        CPPUNIT_ASSERT_EQUAL(n, new_c);
    }
#endif
}
