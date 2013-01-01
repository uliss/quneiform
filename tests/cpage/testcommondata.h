/***************************************************************************
 *   Copyright (C) 2013 by Serge Poltavski                                 *
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

#ifndef TESTCOMMONDATA_H
#define TESTCOMMONDATA_H

#include <cppunit/extensions/HelperMacros.h>

class TestCommonData : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCommonData);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testFlags);
    CPPUNIT_TEST(testNumber);
    CPPUNIT_TEST(testCalcHeight);
    CPPUNIT_TEST(testSetRect);
    CPPUNIT_TEST(testVertexes);
    CPPUNIT_TEST(testIsRect);
    CPPUNIT_TEST(testCopyVertexes);
    CPPUNIT_TEST(testInsertVertex);
    CPPUNIT_TEST(testInsertBottom);
    CPPUNIT_TEST(testInsertTop);
    CPPUNIT_TEST(testInsertLeft);
    CPPUNIT_TEST(testInsertRight);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testFlags();
    void testNumber();
    void testCalcHeight();
    void testSetRect();
    void testVertexes();
    void testIsRect();
    void testCopyVertexes();
    void testInsertVertex();
    void testInsertBottom();
    void testInsertTop();
    void testInsertLeft();
    void testInsertRight();
};



#endif // TESTCOMMONDATA_H
