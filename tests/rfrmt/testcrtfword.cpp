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
#include "testcrtfword.h"
#define private public
#include "rfrmt/crtfword.h"
#include "rfrmt/crtfchar.h"
#include <common/tostring.h>
using namespace CIF;
CPPUNIT_TEST_SUITE_REGISTRATION( TestCRtfWord);

void TestCRtfWord::testInit() {
    CIF::CRtfWord wd;
    CPPUNIT_ASSERT(wd.bRect() == Rect());
    CPPUNIT_ASSERT(wd.charCount() == 0);
    CPPUNIT_ASSERT_THROW(wd.charAt(0), std::out_of_range);
    CPPUNIT_ASSERT_THROW(wd.firstChar(), std::out_of_range);
    CPPUNIT_ASSERT_THROW(wd.lastChar(), std::out_of_range);
    CPPUNIT_ASSERT(wd.fontNumber() == 0);
    CPPUNIT_ASSERT(wd.idealFontSize() == 0);
    CPPUNIT_ASSERT(!wd.isSpelled());
    CPPUNIT_ASSERT(wd.realFontSize() == 0);
    CPPUNIT_ASSERT(wd.probability() == 0);
    const std::string s_empty;
    CPPUNIT_ASSERT_EQUAL(s_empty, wd.toString());
}

void TestCRtfWord::testCharBRect() {
    CRtfWord wd;
    const Rect r_empty;
    CPPUNIT_ASSERT_EQUAL(r_empty, wd.charsBRect());
    wd.addChar(new CRtfChar);
    CPPUNIT_ASSERT_EQUAL(r_empty, wd.charsBRect());
    Rect brect(Point(1, 2), Point(11, 12));
    wd.firstChar()->setIdealRect(brect);
    // only real rectangle is counted
    CPPUNIT_ASSERT_EQUAL(r_empty, wd.charsBRect());
    wd.firstChar()->setRealRect(brect);
    CPPUNIT_ASSERT_EQUAL(brect, wd.charsBRect());
    wd.addChar(new CRtfChar);
    const Point pt_empty;
    CPPUNIT_ASSERT_EQUAL(Rect(pt_empty, Point(11, 12)), wd.charsBRect());
}

void TestCRtfWord::testCalcBRect() {
    CRtfWord wd;
    wd.addChar(new CRtfChar);
    CPPUNIT_ASSERT(wd.charCount() == 1);
    CPPUNIT_ASSERT(wd.firstChar());
    CPPUNIT_ASSERT(wd.lastChar());
    CPPUNIT_ASSERT(wd.charAt(0));

    Rect r_empty;
    CPPUNIT_ASSERT_EQUAL(r_empty, wd.bRect());
    Rect brect(Point(1, 2), Point(11, 12));
    wd.firstChar()->setIdealRect(brect);
    CPPUNIT_ASSERT_EQUAL(r_empty, wd.bRect());
    wd.firstChar()->setIdealRect(brect);
    CPPUNIT_ASSERT_EQUAL(r_empty, wd.bRect());

    CPPUNIT_ASSERT_THROW(wd.calcBRect(), std::runtime_error);
}

void TestCRtfWord::testCharSpelling() {
    CRtfWord wd;
    CPPUNIT_ASSERT(wd.charSpelling());
    wd.addChar(new CRtfChar);
    CPPUNIT_ASSERT(!wd.charSpelling());
    wd.firstChar()->setSpelled(true);
    CPPUNIT_ASSERT(wd.charSpelling());
}

void TestCRtfWord::testCharProbability() {
    CRtfWord wd;

    CPPUNIT_ASSERT(wd.charProbability() == Letter::LOWEST_PROBABILITY);
    wd.addChar(new CRtfChar);
    CPPUNIT_ASSERT_THROW(wd.charProbability(), std::out_of_range);
    wd.firstChar()->addVersion(Letter(' ', Letter::LOWEST_PROBABILITY));
    CPPUNIT_ASSERT(wd.charProbability() == Letter::LOWEST_PROBABILITY);
    wd.firstChar()->first().setProbability(200);
    CPPUNIT_ASSERT(wd.charProbability() == 200);
    wd.addChar(new CRtfChar);
    wd.lastChar()->addVersion(Letter(' ', 100));
    CPPUNIT_ASSERT(wd.charProbability() == 100);
}

void TestCRtfWord::testRotateRect() {
    Rect r1(Point(10, 10), 100, 200);
    Rect r2 = r1;

    CRtfWord::rotateRect(r2, 0, 300, 400);
    CPPUNIT_ASSERT_EQUAL(r1, r2);

    // invalid angle
    CPPUNIT_ASSERT_THROW(CRtfWord::rotateRect(r2, 15, 100, 200), CRtfWord::Exception);

    //90
    r2 = r1;
    CRtfWord::rotateRect(r2, 90, 300, 400);
    CPPUNIT_ASSERT_EQUAL(r1.width(), r2.height());
    CPPUNIT_ASSERT_EQUAL(r1.height(), r2.width());
    // TODO write more tests

    //270
    r2 = r1;
    CRtfWord::rotateRect(r2, 270, 300, 400);
    CPPUNIT_ASSERT_EQUAL(r1.width(), r2.height());
    CPPUNIT_ASSERT_EQUAL(r1.height(), r2.width());
    // TODO write more tests

    //180
    r2 = r1;
    CRtfWord::rotateRect(r2, 180, 300, 400);
    CPPUNIT_ASSERT_EQUAL(r1.width(), r2.width());
    CPPUNIT_ASSERT_EQUAL(r1.height(), r2.height());
    // TODO write more tests
}

void TestCRtfWord::testSet() {
    CRtfWord word;
    std::string str("test");
    word.set(str);
    CPPUNIT_ASSERT(word.charCount() == str.size());
    CPPUNIT_ASSERT_EQUAL(str, word.toString());
}

void TestCRtfWord::testClearChars() {
    CRtfWord wd;
    wd.clearChars();
    CPPUNIT_ASSERT(wd.charCount() == 0);
    wd.addChar('a');
    wd.addChar('b');
    CPPUNIT_ASSERT(wd.charCount() == 2);
    wd.clearChars();
    CPPUNIT_ASSERT(wd.charCount() == 0);
}

void TestCRtfWord::testEndsWith() {
    CRtfWord wd;
    CPPUNIT_ASSERT(!wd.endsWith('a'));
    wd.addChar('b');
    CPPUNIT_ASSERT(!wd.endsWith('a'));
    wd.addChar('a');
    CPPUNIT_ASSERT(wd.endsWith('a'));

    wd.clearChars();
    wd.addChar('a');
    CPPUNIT_ASSERT(wd.endsWith('a'));
}

void TestCRtfWord::testToString() {
    CRtfWord wd;
    const std::string s_empty;
    CPPUNIT_ASSERT_EQUAL(s_empty, wd.toString());
    wd.addChar('a');
    CPPUNIT_ASSERT_EQUAL(std::string("a"), wd.toString());
    wd.addChar('b');
    CPPUNIT_ASSERT_EQUAL(std::string("ab"), wd.toString());
    wd.clearChars();
    CPPUNIT_ASSERT_EQUAL(s_empty, wd.toString());
}

void TestCRtfWord::testStartsWith() {
    CRtfWord wd;
    CPPUNIT_ASSERT(!wd.startsWith('1'));
    wd.addChar('2');
    CPPUNIT_ASSERT(wd.startsWith('2'));
    CPPUNIT_ASSERT(wd.endsWith('2'));
    CPPUNIT_ASSERT(!wd.startsWith('1'));

    wd.firstChar()->first().setChar('1');
    CPPUNIT_ASSERT(wd.startsWith('1'));
    wd.addChar('2');
    CPPUNIT_ASSERT(wd.startsWith('1'));
    CPPUNIT_ASSERT(wd.endsWith('2'));
}

void TestCRtfWord::testCharTotalLength() {
    CRtfWord wd;
    Rect rect;

    CPPUNIT_ASSERT(wd.charTotalLength() == 0);
    wd.addChar('t');
    CPPUNIT_ASSERT(wd.charCount() == 1);
    CPPUNIT_ASSERT(wd.charTotalLength() == 0);

    rect.setWidth(100);
    CPPUNIT_ASSERT(rect.width() == 100);
    wd.lastChar()->setRealRect(rect);
    CPPUNIT_ASSERT_EQUAL(100, wd.charTotalLength());

    wd.addChar('e');
    rect.setWidth(-10);
    wd.lastChar()->setRealRect(rect);
    CPPUNIT_ASSERT_EQUAL(100, wd.charTotalLength());

    wd.addChar('s');
    rect.setWidth(50);
    wd.lastChar()->setRealRect(rect);
    CPPUNIT_ASSERT_EQUAL(150, wd.charTotalLength());
}
