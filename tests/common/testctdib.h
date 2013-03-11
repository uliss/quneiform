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

#ifndef TESTCTDIB_H
#define TESTCTDIB_H

#include <cppunit/extensions/HelperMacros.h>

class TestCTDIB : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCTDIB);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testSaveToBMP);
    CPPUNIT_TEST(testFill);
    CPPUNIT_TEST(testMapTo24);
    CPPUNIT_TEST(testMapTo32);
    CPPUNIT_TEST(testSetPixelColor);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testSaveToBMP();
    void testFill();
    void testMapTo32();
    void testMapTo24();
    void testSetPixelColor();
};

#endif // TESTCTDIB_H
