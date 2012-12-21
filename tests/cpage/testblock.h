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

#ifndef TESTBLOCK_H
#define TESTBLOCK_H

#include <cppunit/extensions/HelperMacros.h>

class TestBlock : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestBlock);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testSet);
    CPPUNIT_TEST(testCopy);
    CPPUNIT_TEST(testGet);
    CPPUNIT_TEST(testCompare);
    CPPUNIT_TEST(testSave);
    CPPUNIT_TEST(testRestore);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testSet();
    void testGet();
    void testCopy();
    void testCompare();
    void testSave();
    void testRestore();
};


#endif // TESTBLOCK_H
