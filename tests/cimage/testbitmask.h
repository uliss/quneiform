/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#ifndef TESTBITMASK_H
#define TESTBITMASK_H

#include <cppunit/extensions/HelperMacros.h>

class TestBitMask : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestBitMask);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testFill);
    CPPUNIT_TEST(testInvert);
    CPPUNIT_TEST(testFillRect);
    CPPUNIT_TEST(testSet);
    CPPUNIT_TEST(testUnset);
    CPPUNIT_TEST(testApply1Bit);
    CPPUNIT_TEST(testApply8Bit);
    CPPUNIT_TEST(testApply24Bit);
    CPPUNIT_TEST(testApply32Bit);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testFill();
    void testInvert();
    void testFillRect();
    void testSet();
    void testUnset();
    void testApply1Bit();
    void testApply8Bit();
    void testApply24Bit();
    void testApply32Bit();
};

#endif // TESTBITMASK_H
