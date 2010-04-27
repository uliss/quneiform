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

#ifndef TESTCRTFFRAGMENT_H_
#define TESTCRTFFRAGMENT_H_

#include <cppunit/extensions/HelperMacros.h>

class TestCRtfFragment: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCRtfFragment);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testAddString);
    CPPUNIT_TEST(testClearStrings);
    CPPUNIT_TEST(testFontSizePenalty);
    CPPUNIT_TEST(testCharCount);
    CPPUNIT_TEST(testCharTotalLength);
    CPPUNIT_TEST(testCalcMaxCharDistance);
    CPPUNIT_TEST(testMinStringLeftBorder);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testAddString();
    void testClearStrings();
    void testFontSizePenalty();
    void testCharCount();
    void testCharTotalLength();
    void testCalcMaxCharDistance();
    void testMinStringLeftBorder();
};

#endif /* TESTCRTFFRAGMENT_H_ */
