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

#ifndef TESTPUMA_H_
#define TESTPUMA_H_

#include <cppunit/extensions/HelperMacros.h>

class TestCRtfWord: public CppUnit::TestFixture
{
        CPPUNIT_TEST_SUITE(TestCRtfWord);
        CPPUNIT_TEST(testInit);
        CPPUNIT_TEST(testCalcBRect);
        CPPUNIT_TEST(testCharBRect);
        CPPUNIT_TEST(testCharSpelling);
        CPPUNIT_TEST(testCharProbability);
        CPPUNIT_TEST(testRotateRect);
        CPPUNIT_TEST(testClearChars);
        CPPUNIT_TEST(testToString);
        CPPUNIT_TEST(testStartsWith);
        CPPUNIT_TEST(testCharTotalLength);
        CPPUNIT_TEST(testSet);
        CPPUNIT_TEST(testEndsWith);
        CPPUNIT_TEST(testSetDrawCallback);
        CPPUNIT_TEST_SUITE_END();
    public:
        void testInit();
        void testCalcBRect();
        void testCharBRect();
        void testCharSpelling();
        void testCharProbability();
        void testClearChars();
        void testEndsWith();
        void testRotateRect();
        void testToString();
        void testStartsWith();
        void testSet();
        void testCharTotalLength();
        void testSetDrawCallback();
};

#endif /* TESTPUMA_H_ */
