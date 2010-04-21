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
}

void TestCRtfWord::testCharBRect() {
    CRtfWord wd;
    CPPUNIT_ASSERT_EQUAL(Rect(), wd.charsBRect());
    wd.addChar(new CRtfChar);
    CPPUNIT_ASSERT_EQUAL(Rect(), wd.charsBRect());
    Rect brect(Point(1, 2), Point(11, 12));
    wd.firstChar()->setIdealRect(brect);
    // only real rectangle is counted
    CPPUNIT_ASSERT_EQUAL(Rect(), wd.charsBRect());
    wd.firstChar()->setRealRect(brect);
    CPPUNIT_ASSERT_EQUAL(brect, wd.charsBRect());
    wd.addChar(new CRtfChar);
    CPPUNIT_ASSERT_EQUAL(Rect(Point(), Point(11, 12)), wd.charsBRect());
}

void TestCRtfWord::testCalcBRect() {
    CRtfWord wd;
    wd.addChar(new CRtfChar);
    CPPUNIT_ASSERT(wd.charCount() == 1);
    CPPUNIT_ASSERT(wd.firstChar());
    CPPUNIT_ASSERT(wd.lastChar());
    CPPUNIT_ASSERT(wd.charAt(0));

    CPPUNIT_ASSERT_EQUAL(Rect(), wd.bRect());
    Rect brect(Point(1, 2), Point(11, 12));
    wd.firstChar()->setIdealRect(brect);
    CPPUNIT_ASSERT_EQUAL(Rect(), wd.bRect());
    wd.firstChar()->setIdealRect(brect);
    CPPUNIT_ASSERT_EQUAL(Rect(), wd.bRect());

    //wd.calcBRect();

    //CPPUNIT_ASSERT_EQUAL(brect, wd.bRect());
}

void TestCRtfWord::testCharSpelling() {
    CRtfWord wd;
    CPPUNIT_ASSERT(wd.charSpelling());
    wd.addChar(new CRtfChar);
    CPPUNIT_ASSERT(!wd.charSpelling());
    wd.firstChar()->m_bFlg_spell = TRUE;
    CPPUNIT_ASSERT(wd.charSpelling());
}

void TestCRtfWord::testCharProbability() {
    CRtfWord wd;
    CPPUNIT_ASSERT(wd.charMinProbability() == Letter::LOWEST_PROBABILITY);
//    wd.addChar(new CRtfChar);
//    CPPUNIT_ASSERT(!wd.charSpelling());
//    wd.firstChar()->m_bFlg_spell = TRUE;
//    CPPUNIT_ASSERT(wd.charSpelling());
}
