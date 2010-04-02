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
#include <ced/cedchar.h>
#include <ced/cedline.h>
using namespace CIF;

void TestCedLine::testInit() {
    CEDLine ln;
    CPPUNIT_ASSERT(0 == ln.currentChar());
    CPPUNIT_ASSERT(0 == ln.charCount());
    CPPUNIT_ASSERT(0 == ln.internalNumber());
    CPPUNIT_ASSERT(0 == ln.parentNumber());
    CPPUNIT_ASSERT(0 == ln.first());
    CPPUNIT_ASSERT(0 == ln.next());
    CPPUNIT_ASSERT(0 == ln.prev());
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
    CPPUNIT_ASSERT(0 == chr->next);
    CPPUNIT_ASSERT(0 == chr->prev);

    CEDChar * chr2 = ln.insertChar();
    CPPUNIT_ASSERT(chr2);
    CPPUNIT_ASSERT_EQUAL(2, ln.charCount());
    CPPUNIT_ASSERT_EQUAL(chr2->parentNumber(), ln.internalNumber());
    CPPUNIT_ASSERT_EQUAL(chr, ln.charAt(0));
    CPPUNIT_ASSERT_EQUAL(chr2, ln.charAt(1));
    CPPUNIT_ASSERT_EQUAL(chr2, ln.currentChar());
    CPPUNIT_ASSERT_EQUAL(chr, ln.first());
    CPPUNIT_ASSERT_EQUAL(chr2, chr->next);
    CPPUNIT_ASSERT_EQUAL(chr, chr2->prev);
    CPPUNIT_ASSERT(0 == chr2->next);
    CPPUNIT_ASSERT(0 == chr->prev);

    delete chr;
    delete chr2;
}

void TestCedLine::testCharAt() {
    CEDLine ln;
    CEDChar * chr = ln.insertChar();
    CPPUNIT_ASSERT_EQUAL(chr, ln.charAt(0));
    CPPUNIT_ASSERT(0 == ln.charAt(-1));
    CPPUNIT_ASSERT(0 == ln.charAt(19));

    delete chr;
}

void TestCedLine::testNextChar() {
    CEDLine ln;
    ln.internal_number_ = 1234;
    CPPUNIT_ASSERT(0 == ln.nextChar());
    ln.insertChar();

    CPPUNIT_ASSERT(0 == ln.nextChar());
    ln.insertChar();
    ln.setCurrentChar(0);
    CPPUNIT_ASSERT_EQUAL(ln.charAt(1), ln.nextChar());

    ln.charAt(1)->setParentNumber(123);
    CPPUNIT_ASSERT(ln.charAt(1) != ln.nextChar());
}

void TestCedLine::testPrevChar() {
    CEDLine ln;
    CPPUNIT_ASSERT(0 == ln.prevChar());
}
