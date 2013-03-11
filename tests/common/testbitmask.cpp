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

#include <sstream>

#include "testbitmask.h"
#include "common/bitmask.h"
#include "common/tostring.h"
#include "common/helper.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestBitMask);

using namespace cf;

void TestBitMask::testInit()
{
    BitMask bm(Size(2, 2));
    CPPUNIT_ASSERT_EQUAL(Size(2, 2), bm.size());
    CPPUNIT_ASSERT(!bm.isSet(0, 0));
    CPPUNIT_ASSERT(!bm.isSet(1, 0));
    CPPUNIT_ASSERT(!bm.isSet(0, 1));
    CPPUNIT_ASSERT(!bm.isSet(1, 1));

    BitMask bm1(1, 2);
    CPPUNIT_ASSERT_EQUAL(Size(1, 2), bm1.size());
    CPPUNIT_ASSERT(!bm1.isSet(0, 0));
    CPPUNIT_ASSERT(!bm1.isSet(1, 1));

    uchar data = 0x7F; // 0b01111111
    BitMask bm2(1, 4, &data);
    CPPUNIT_ASSERT_EQUAL(Size(1, 4), bm2.size());
    CPPUNIT_ASSERT(!bm2.isSet(0, 0));
    CPPUNIT_ASSERT(bm2.isSet(0, 1));
    CPPUNIT_ASSERT(bm2.isSet(0, 2));
    CPPUNIT_ASSERT(bm2.isSet(0, 3));

}

void TestBitMask::testRevert()
{
    BitMask m(2, 2);
    m.invert();
    CPPUNIT_ASSERT(m.isSet(0, 0));
    CPPUNIT_ASSERT(m.isSet(1, 0));
    CPPUNIT_ASSERT(m.isSet(0, 1));
    CPPUNIT_ASSERT(m.isSet(1, 1));

    m.unset(1, 1);
    m.unset(0, 0);
    m.invert();

    CPPUNIT_ASSERT(m.isSet(0, 0));
    CPPUNIT_ASSERT(!m.isSet(1, 0));
    CPPUNIT_ASSERT(!m.isSet(0, 1));
    CPPUNIT_ASSERT(m.isSet(1, 1));
}

void TestBitMask::testFill()
{
    BitMask m(2, 2);
    m.fill(true);
    CPPUNIT_ASSERT(m.isSet(0, 0));
    CPPUNIT_ASSERT(m.isSet(1, 0));
    CPPUNIT_ASSERT(m.isSet(0, 1));
    CPPUNIT_ASSERT(m.isSet(1, 1));
    m.fill(false);
    CPPUNIT_ASSERT(!m.isSet(0, 0));
    CPPUNIT_ASSERT(!m.isSet(1, 0));
    CPPUNIT_ASSERT(!m.isSet(0, 1));
    CPPUNIT_ASSERT(!m.isSet(1, 1));
}

void TestBitMask::testFillRect()
{
    BitMask m(3, 3);
    CPPUNIT_ASSERT(!m.isSet(0, 0)); //
    CPPUNIT_ASSERT(!m.isSet(1, 0));
    CPPUNIT_ASSERT(!m.isSet(2, 0));
    CPPUNIT_ASSERT(!m.isSet(0, 1)); //
    CPPUNIT_ASSERT(!m.isSet(1, 1));
    CPPUNIT_ASSERT(!m.isSet(2, 1));
    CPPUNIT_ASSERT(!m.isSet(0, 2)); //
    CPPUNIT_ASSERT(!m.isSet(1, 2));
    CPPUNIT_ASSERT(!m.isSet(2, 2));

    m.fillRect(cf::Rect(0, 0, 3, 3), true);
    CPPUNIT_ASSERT(m.isSet(0, 0)); //
    CPPUNIT_ASSERT(m.isSet(1, 0));
    CPPUNIT_ASSERT(m.isSet(2, 0));
    CPPUNIT_ASSERT(m.isSet(0, 1)); //
    CPPUNIT_ASSERT(m.isSet(1, 1));
    CPPUNIT_ASSERT(m.isSet(2, 1));
    CPPUNIT_ASSERT(m.isSet(0, 2)); //
    CPPUNIT_ASSERT(m.isSet(1, 2));
    CPPUNIT_ASSERT(m.isSet(2, 2));

    m.fillRect(cf::Rect(0, 0, 3, 3), false);
    CPPUNIT_ASSERT(!m.isSet(0, 0)); //
    CPPUNIT_ASSERT(!m.isSet(1, 0));
    CPPUNIT_ASSERT(!m.isSet(2, 0));
    CPPUNIT_ASSERT(!m.isSet(0, 1)); //
    CPPUNIT_ASSERT(!m.isSet(1, 1));
    CPPUNIT_ASSERT(!m.isSet(2, 1));
    CPPUNIT_ASSERT(!m.isSet(0, 2)); //
    CPPUNIT_ASSERT(!m.isSet(1, 2));
    CPPUNIT_ASSERT(!m.isSet(2, 2));

    m.fillRect(cf::Rect(1, 1, 3, 3), true);
    CPPUNIT_ASSERT(!m.isSet(0, 0)); //
    CPPUNIT_ASSERT(!m.isSet(1, 0));
    CPPUNIT_ASSERT(!m.isSet(2, 0));
    CPPUNIT_ASSERT(!m.isSet(0, 1)); //
    CPPUNIT_ASSERT(m.isSet(1, 1));
    CPPUNIT_ASSERT(m.isSet(2, 1));
    CPPUNIT_ASSERT(!m.isSet(0, 2)); //
    CPPUNIT_ASSERT(m.isSet(1, 2));
    CPPUNIT_ASSERT(m.isSet(2, 2));

    m.fillRect(cf::Rect(2, 0, 3, 3), true);
    CPPUNIT_ASSERT(!m.isSet(0, 0)); //
    CPPUNIT_ASSERT(!m.isSet(1, 0));
    CPPUNIT_ASSERT(m.isSet(2, 0));
    CPPUNIT_ASSERT(!m.isSet(0, 1)); //
    CPPUNIT_ASSERT(m.isSet(1, 1));
    CPPUNIT_ASSERT(m.isSet(2, 1));
    CPPUNIT_ASSERT(!m.isSet(0, 2)); //
    CPPUNIT_ASSERT(m.isSet(1, 2));
    CPPUNIT_ASSERT(m.isSet(2, 2));
}

void TestBitMask::testOutput()
{
    BitMask bm(3, 3);
    std::ostringstream buf1;
    buf1 << bm;

    CPPUNIT_ASSERT_EQUAL(std::string("000\n000\n000\n"), buf1.str());
    bm.invert();
    std::ostringstream buf2;
    buf2 << bm;
    CPPUNIT_ASSERT_EQUAL(std::string("111\n111\n111\n"), buf2.str());
}

void TestBitMask::testSetUnset()
{
    BitMask m(3, 3);
    CPPUNIT_ASSERT(!m.isSet(0, 0)); //
    CPPUNIT_ASSERT(!m.isSet(1, 0));
    CPPUNIT_ASSERT(!m.isSet(2, 0));
    CPPUNIT_ASSERT(!m.isSet(0, 1)); //
    CPPUNIT_ASSERT(!m.isSet(1, 1));
    CPPUNIT_ASSERT(!m.isSet(2, 1));
    CPPUNIT_ASSERT(!m.isSet(0, 2)); //
    CPPUNIT_ASSERT(!m.isSet(1, 2));
    CPPUNIT_ASSERT(!m.isSet(2, 2));

    m.set(3, 3);
    CPPUNIT_ASSERT(!m.isSet(0, 0)); //
    CPPUNIT_ASSERT(!m.isSet(1, 0));
    CPPUNIT_ASSERT(!m.isSet(2, 0));
    CPPUNIT_ASSERT(!m.isSet(0, 1)); //
    CPPUNIT_ASSERT(!m.isSet(1, 1));
    CPPUNIT_ASSERT(!m.isSet(2, 1));
    CPPUNIT_ASSERT(!m.isSet(0, 2)); //
    CPPUNIT_ASSERT(!m.isSet(1, 2));
    CPPUNIT_ASSERT(!m.isSet(2, 2));

    m.fill(true);
    m.unset(3, 4);

    CPPUNIT_ASSERT(m.isSet(0, 0)); //
    CPPUNIT_ASSERT(m.isSet(1, 0));
    CPPUNIT_ASSERT(m.isSet(2, 0));
    CPPUNIT_ASSERT(m.isSet(0, 1)); //
    CPPUNIT_ASSERT(m.isSet(1, 1));
    CPPUNIT_ASSERT(m.isSet(2, 1));
    CPPUNIT_ASSERT(m.isSet(0, 2)); //
    CPPUNIT_ASSERT(m.isSet(1, 2));
    CPPUNIT_ASSERT(m.isSet(2, 2));
}
