/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#include <sstream>

#include "testletter.h"
#include "../test_common.h"
#include "common/letter.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestLetter);

using namespace cf;

void TestLetter::testInit()
{
    Letter lt;
    CPPUNIT_ASSERT_EQUAL((int) Letter::LOWEST_PROBABILITY, (int) lt.probability());
    CPPUNIT_ASSERT_EQUAL((uchar) '\x0', lt.getChar());

    lt.setChar('-');
    CPPUNIT_ASSERT_EQUAL((uchar) '-', lt.getChar());
    CPPUNIT_ASSERT(lt.isHyphen());

    lt.setProbability(10);
    CPPUNIT_ASSERT_EQUAL((int) 10, (int) lt.probability());

    lt.setChar(10);
    lt.normalizeNonPrintable();
    CPPUNIT_ASSERT_EQUAL((int) ' ', (int) lt.getChar());

    Letter lt2;
    lt2 = lt;
    CPPUNIT_ASSERT_EQUAL(lt.getChar(), lt2.getChar());
    CPPUNIT_ASSERT_EQUAL(lt.probability(), lt2.probability());

    CPPUNIT_ASSERT_EQUAL(lt2, lt);
}

void TestLetter::testOutput()
{
    Letter lt('A', 12);
    std::ostringstream buf;
    buf << lt;
    CPPUNIT_ASSERT_EQUAL(std::string("Letter: 'A' (65) prob: 12"), buf.str());
}

void TestLetter::testSerialize()
{
#ifdef CF_SERIALIZE
    const Letter lt('A', 124);

    const char * TXT = "serialize_letter.txt";
    writeToTextArchive(TXT, lt);

    Letter new_lt;
    CPPUNIT_ASSERT(lt != new_lt);
    readFromTextArchive(TXT, new_lt);

    CPPUNIT_ASSERT(lt == new_lt);
#endif
}

void TestLetter::testSerializeXML()
{
#ifdef CF_SERIALIZE
    const Letter lt('A', 124);

    const char * fname = "serialize_letter.xml";
    writeToXmlArchive(fname, "letter", lt);

    Letter new_lt;
    CPPUNIT_ASSERT(lt != new_lt);
    readFromXmlArchive(fname, "letter", new_lt);

    CPPUNIT_ASSERT(lt == new_lt);
#endif
}
