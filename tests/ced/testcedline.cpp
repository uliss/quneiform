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
#include "testcedline.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCedLine);
#define private public
#include <iostream>
#include <fstream>
#include <ced/cedchar.h>
#include <ced/cedline.h>
#include <ced/cedarchive.h>
using namespace CIF;

void TestCedLine::testInit() {
    CEDLine ln;
    CPPUNIT_ASSERT(0 == ln.charCount());
    CPPUNIT_ASSERT(0 == ln.internalNumber());
    CPPUNIT_ASSERT(0 == ln.parentNumber());
    CPPUNIT_ASSERT_THROW(ln.first(), std::out_of_range);
}

void TestCedLine::testInsertChar() {
    CEDLine ln;
    ln.internal_number_ = 1234;
    CEDChar * chr = ln.insertChar();

    CPPUNIT_ASSERT(chr);
    CPPUNIT_ASSERT_EQUAL(1, ln.charCount());
    CPPUNIT_ASSERT_EQUAL(ln.internalNumber(), chr->parentNumber());
    CPPUNIT_ASSERT_EQUAL(chr, ln.first());
    CPPUNIT_ASSERT_EQUAL(chr, ln.charAt(0));

    CEDChar * chr2 = ln.insertChar();
    CPPUNIT_ASSERT(chr2);
    CPPUNIT_ASSERT_EQUAL(2, ln.charCount());
    CPPUNIT_ASSERT_EQUAL(chr2->parentNumber(), ln.internalNumber());
    CPPUNIT_ASSERT_EQUAL(chr, ln.charAt(0));
    CPPUNIT_ASSERT_EQUAL(chr2, ln.charAt(1));
    CPPUNIT_ASSERT_EQUAL(chr, ln.first());

    CEDChar * chr3 = ln.insertChar();
    CPPUNIT_ASSERT(chr3);
    CPPUNIT_ASSERT_EQUAL(3, ln.charCount());
    CPPUNIT_ASSERT_EQUAL(chr3->parentNumber(), ln.internalNumber());
    CPPUNIT_ASSERT_EQUAL(chr, ln.charAt(0));
    CPPUNIT_ASSERT_EQUAL(chr2, ln.charAt(1));
    CPPUNIT_ASSERT_EQUAL(chr3, ln.charAt(2));
    CPPUNIT_ASSERT_EQUAL(chr, ln.first());
}

void TestCedLine::testCharAt() {
    CEDLine ln;
    CEDChar * chr = ln.insertChar();
    CPPUNIT_ASSERT_EQUAL(chr, ln.charAt(0));
    CPPUNIT_ASSERT_THROW(ln.charAt(-1), std::out_of_range);
    CPPUNIT_ASSERT_THROW(ln.charAt(19), std::out_of_range);
}

void TestCedLine::testSerialize() {
#ifdef CF_SERIALIZE
    CEDLine ln;
    ln.setHardBreak(true);
    ln.setDefaultFontHeight(11);

    ln.insertChar(new CEDChar);
    ln.charAt(0)->setFontHeight(12);
    ln.charAt(0)->setForegroundColor(Color(0, 100, 200));

    const char * fname = "serialize_cedline.txt";

    // save data to archive
    {
        std::ofstream ofs(fname);
        CEDOutputArchive oa(ofs);
        // write class instance to archive
        oa << ln;
    }

    CEDLine new_line;
    {
        // create and open an archive for input
        std::ifstream ifs(fname);
        assert(ifs);
        CEDInputArchive ia(ifs);
        // read class state from archive
        ia >> new_line;

        CPPUNIT_ASSERT_EQUAL(new_line.charCount(), ln.charCount());
        CPPUNIT_ASSERT(new_line.charAt(0)->fontHeight() == 12);
        CPPUNIT_ASSERT(new_line.hardBreak());
    }

#endif
}
