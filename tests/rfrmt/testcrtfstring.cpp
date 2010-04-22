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
#include "testcrtfstring.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCRtfString);
#include "rfrmt/crtfstring.h"
#include "rfrmt/crtfword.h"
#include "rfrmt/crtfchar.h"
#include "rfrmt/creatertf.h"
using namespace CIF;

void TestCRtfString::testInit() {
    CRtfString str;
    CPPUNIT_ASSERT(str.wordCount() == 0);
    CPPUNIT_ASSERT_THROW(str.firstWord(), std::out_of_range);
    CPPUNIT_ASSERT_THROW(str.wordAt(0), std::out_of_range);
}

void TestCRtfString::testClearWords() {
    CRtfString str;
    CPPUNIT_ASSERT(str.wordCount() == 0);
    str.clearWords();
    str.addWord(new CRtfWord);
    CPPUNIT_ASSERT(str.wordCount() == 1);
    str.clearWords();
    CPPUNIT_ASSERT(str.wordCount() == 0);
}

void TestCRtfString::testToString() {
    CRtfString str;
    CPPUNIT_ASSERT_EQUAL(std::string(), str.toString());
    str.addWord(new CRtfWord);
    CPPUNIT_ASSERT_EQUAL(std::string(), str.toString());
    str.firstWord()->addChar(new CRtfChar('a', 100));
    str.firstWord()->addChar(new CRtfChar('b', 100));
    CPPUNIT_ASSERT_EQUAL(std::string("ab"), str.toString());

    str.addWord(new CRtfWord);
    str.lastWord()->addChar(new CRtfChar('c', 100));
    str.lastWord()->addChar(new CRtfChar('d', 100));
    CPPUNIT_ASSERT_EQUAL(std::string("ab cd"), str.toString());
}

void TestCRtfString::testRealLength() {
    CRtfString str;
    CPPUNIT_ASSERT(str.realLength() == 0);
    // now it's a stub
    // so always returns 0
    str.addWord(new CRtfWord);
    CPPUNIT_ASSERT(str.realLength() == 0);
    str.firstWord()->addChar(new CRtfChar('a', 100));
    CPPUNIT_ASSERT(str.realLength() == 0);
}

void TestCRtfString::testFirstChar() {
    CRtfString str;
    CPPUNIT_ASSERT(str.firstChar() == NULL);
    str.addWord(new CRtfWord);
    CPPUNIT_ASSERT(str.firstChar() == NULL);
    CRtfChar * chr = new CRtfChar;
    str.firstWord()->addChar(chr);
    CPPUNIT_ASSERT(str.firstChar() == chr);
}

void TestCRtfString::testStartsWith() {
    CRtfString str;
    str.addWord(new CRtfWord);
    CPPUNIT_ASSERT(!str.startsWith('1'));
    std::string chars("123");
    for (size_t i = 0; i < chars.length(); i++) {
        str.firstWord()->addChar(new CRtfChar(chars[i], 0));
    }
    CPPUNIT_ASSERT(str.startsWith('1'));
    CPPUNIT_ASSERT(str.endsWith('3'));
}

void TestCRtfString::testStartsWithDigit() {
    CRtfString str;
    CPPUNIT_ASSERT(!str.startsWithDigit());
    str.addWord(new CRtfWord);
    CPPUNIT_ASSERT(!str.startsWithDigit());
    CRtfChar * chr = new CRtfChar(' ', 0);
    str.firstWord()->addChar(chr);
    str.firstWord()->addChar(new CRtfChar(' ', 0));
    CPPUNIT_ASSERT(str.firstChar() == chr);
    CPPUNIT_ASSERT(!str.startsWithDigit());

    std::string chars("0123456789");
    for (size_t i = 0; i < chars.length(); i++) {
        chr->first().setChar(chars[i]);
        CPPUNIT_ASSERT(str.startsWithDigit());
    }

    std::string non_chars("x!@#$%^&*()q-weryuiopsdfghjklcvbnm,");
    for (size_t i = 0; i < non_chars.length(); i++) {
        chr->first().setChar(non_chars[i]);
        CPPUNIT_ASSERT(!str.startsWithDigit());
    }
}

void TestCRtfString::testStartWithDash() {
    CRtfString str;
    CPPUNIT_ASSERT(!str.startsWithDash());
    str.addWord(new CRtfWord);
    CPPUNIT_ASSERT(!str.startsWithDash());
    CRtfChar * chr = new CRtfChar(' ', 0);
    str.firstWord()->addChar(chr);
    CPPUNIT_ASSERT(!str.startsWithDash());
    chr->first().setChar('-');
    CPPUNIT_ASSERT(str.startsWithDash());
    chr->first().setChar(TIRE);
    CPPUNIT_ASSERT(str.startsWithDash());

    std::string non_dash("x!@#$%^&*12313214()qweryuiopsdfghjklcvbnm,");
    for (size_t i = 0; i < non_dash.length(); i++) {
        chr->first().setChar(non_dash[i]);
        CPPUNIT_ASSERT(!str.startsWithDash());
    }
}
