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
#include "testsize.h"
#include "../test_common.h"
#include <common/size.h>
using namespace cf;
CPPUNIT_TEST_SUITE_REGISTRATION(TestSize);

void TestSize::testIsValid() {
    Size sz;
    CPPUNIT_ASSERT(!sz.isValid());

    Size sz1(0, 1);
    CPPUNIT_ASSERT(sz.isValid());

    Size sz2(1, 0);
    CPPUNIT_ASSERT(sz.isValid());

    Size sz3(-1, -2);
    CPPUNIT_ASSERT(!sz.isValid());
}

void TestSize::testSerialize() {
#ifdef CF_SERIALIZE
    const Size sz(-20, -30);

    const char * TXT = "serialize_size.txt";
    writeToTextArchive(TXT, sz);

    Size new_sz;
    CPPUNIT_ASSERT(sz != new_sz);
    readFromTextArchive(TXT, new_sz);

    CPPUNIT_ASSERT(sz == new_sz);
#endif
}

void TestSize::testSerializeXml() {
#ifdef CF_SERIALIZE
    const Size sz(-20, -30);

    const char * XML = "serialize_size.xml";
    writeToXmlArchive(XML, "size", sz);

    Size new_sz;
    CPPUNIT_ASSERT(sz != new_sz);
    readFromXmlArchive(XML, "size", new_sz);

    CPPUNIT_ASSERT(sz == new_sz);
#endif
}
