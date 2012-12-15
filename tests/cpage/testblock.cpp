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

#include "testblock.h"
#include "cpage/block.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestBlock);

using namespace cf::cpage;

void TestBlock::testInit()
{
    Block b;
    CPPUNIT_ASSERT(b.empty());
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b.userNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b.interNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b.flags());
}

void TestBlock::testSet()
{
    Block b;
    b.set((Handle) 0x1, 1, 2);
    CPPUNIT_ASSERT(b.empty());
    CPPUNIT_ASSERT_EQUAL(uint32_t(1), b.userNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b.interNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(2), b.flags());
    CPPUNIT_ASSERT_EQUAL((Handle) 0x1, b.type());

    char data[100] = { 0xff };
    b.set(0, 2, 3, &data, sizeof(data));
    CPPUNIT_ASSERT(!b.empty());
}

void TestBlock::testCopy()
{
    char data[100] = { 0xf0 };
    Block b;
    b.set(0, 2, 3, &data, sizeof(data));

    // copy ctor
    Block b_copy = b;
    CPPUNIT_ASSERT(!b_copy.empty());
    CPPUNIT_ASSERT_EQUAL(uint32_t(2), b.userNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b.interNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(3), b.flags());
}
