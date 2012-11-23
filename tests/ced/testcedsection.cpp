/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include "testcedsection.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDSection);

#include "../test_common.h"
#include "common/tostring.h"
#include "ced/cedpicture.h"
#include "ced/cedchar.h"
#include "ced/cedline.h"
#include "ced/cedparagraph.h"
#include "ced/cedcolumn.h"
#include "ced/cedframe.h"
#include "ced/cedsection.h"
#include "ced/cedarchive.h"

using namespace cf;

void TestCEDSection::testSerialize() {
#ifdef CF_SERIALIZE
    const char * fname = "serialize_cedsection.txt";
    CEDSection sec;
    sec.setLineBetweenColumns(true);
    sec.setSectionBreak(true);
    sec.addColumn(new CEDColumn());

    writeToTextArchive(fname, sec);

    CEDSection new_sec;
    readFromTextArchive(fname, new_sec);

    CPPUNIT_ASSERT_EQUAL(sec.lineBetweenColumns(), new_sec.lineBetweenColumns());
    CPPUNIT_ASSERT_EQUAL(sec.sectionBreak(), new_sec.sectionBreak());
    CPPUNIT_ASSERT(!new_sec.empty());
    CPPUNIT_ASSERT_EQUAL(new_sec.columnCount(), (size_t) 1);
#endif
}

void TestCEDSection::testSerializeXml() {
#ifdef CF_SERIALIZE
    const char * fname = "serialize_cedsection.xml";
    CEDSection sec;
    sec.setLineBetweenColumns(true);
    sec.setSectionBreak(true);

    writeToXmlArchive(fname, "cedsection", sec);

    CEDSection new_sec;
    readFromXmlArchive(fname, "cedsection", new_sec);

    CPPUNIT_ASSERT_EQUAL(sec.lineBetweenColumns(), new_sec.lineBetweenColumns());
    CPPUNIT_ASSERT_EQUAL(sec.sectionBreak(), new_sec.sectionBreak());
#endif
}

void TestCEDSection::testClone()
{
    CEDSection sec;
    sec.setHeaderY(10);
    sec.setFooterY(20);
    sec.setSectionBreak(true);
    sec.setLineBetweenColumns(false);
    sec.setOrientation(1);
    sec.addColInfo(10, 20);

    CEDSection * sec_copy = sec.clone();
    CPPUNIT_ASSERT(sec_copy);
    CPPUNIT_ASSERT_EQUAL(sec.lineBetweenColumns(), sec_copy->lineBetweenColumns());
    CPPUNIT_ASSERT_EQUAL(sec.sectionBreak(), sec_copy->sectionBreak());
    delete sec_copy;
}
