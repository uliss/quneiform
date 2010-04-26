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
#include <stdexcept>
#include "testcrtffragment.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCRtfFragment);
#define private public
#include "rfrmt/crtffragment.h"
#include "rfrmt/crtfstring.h"

using namespace CIF;

void TestCRtfFragment::testInit() {
    CRtfFragment fr;
    CPPUNIT_ASSERT(fr.stringCount() == 0);
    CPPUNIT_ASSERT_THROW(fr.stringAt(0), std::out_of_range);
    CPPUNIT_ASSERT_THROW(fr.firstString(), std::out_of_range);
}

void TestCRtfFragment::testAddString() {
    CRtfFragment fr;
    CPPUNIT_ASSERT(fr.stringCount() == 0);
    CRtfString * str = new CRtfString;
    fr.addString(str);
    CPPUNIT_ASSERT(fr.stringCount() == 1);
    CPPUNIT_ASSERT_EQUAL(str, fr.stringAt(0));
    CPPUNIT_ASSERT_EQUAL(str, fr.firstString());
}

void TestCRtfFragment::testClearStrings() {
    CRtfFragment fr;
    CPPUNIT_ASSERT(fr.stringCount() == 0);
    fr.clearStrings();
    CPPUNIT_ASSERT(fr.stringCount() == 0);
    fr.addString(new CRtfString);
    CPPUNIT_ASSERT(fr.stringCount() == 1);
    fr.addString(new CRtfString);
    CPPUNIT_ASSERT(fr.stringCount() == 2);
    fr.clearStrings();
    CPPUNIT_ASSERT(fr.stringCount() == 0);
}

void TestCRtfFragment::testFontSizePenalty() {
    CRtfFragment fr;
    // penalty for empty fragment
    CPPUNIT_ASSERT(fr.fontSizePenalty(0) == 0);
}
