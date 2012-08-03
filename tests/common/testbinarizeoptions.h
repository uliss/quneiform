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

#ifndef TESTBINARIZEOPTIONS_H
#define TESTBINARIZEOPTIONS_H

#include <cppunit/extensions/HelperMacros.h>

class TestBinarizeOptions : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestBinarizeOptions);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testBoolOptions);
    CPPUNIT_TEST(testFloatOptions);
    CPPUNIT_TEST(testIntOptions);
    CPPUNIT_TEST(testStringOptions);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testBoolOptions();
    void testFloatOptions();
    void testIntOptions();
    void testStringOptions();
};


#endif // TESTBINARIZEOPTIONS_H
