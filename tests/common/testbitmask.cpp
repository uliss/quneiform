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

#include "testbitmask.h"
#include "common/bitmask.h"
#include "common/tostring.h"
#include "common/helper.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestBitMask);

using namespace cf;

#define MASK_EQUAL(w, h, str, mask) {\
    BitMask expected(w, h, str);\
    CPPUNIT_ASSERT_EQUAL(expected, mask);\
    }

void TestBitMask::testInit()
{
    BitMask null;
    CPPUNIT_ASSERT(!null.width());
    CPPUNIT_ASSERT(!null.height());
    CPPUNIT_ASSERT_EQUAL(Size(), null.size());
    CPPUNIT_ASSERT(!null.isSet(0, 0));
    null.fill(true);
    null.fillRect(Rect(-10, -20, 300, 300), false);

    BitMask bm(Size(2, 2));
    CPPUNIT_ASSERT_EQUAL(Size(2, 2), bm.size());
    MASK_EQUAL(2, 2, "0000", bm);
    CPPUNIT_ASSERT(!bm.isSet(0, 0));
    CPPUNIT_ASSERT(!bm.isSet(1, 0));
    CPPUNIT_ASSERT(!bm.isSet(0, 1));
    CPPUNIT_ASSERT(!bm.isSet(1, 1));
    CPPUNIT_ASSERT(!bm.isSet(100, 100));

    BitMask bm1(1, 2);
    CPPUNIT_ASSERT_EQUAL(Size(1, 2), bm1.size());
    MASK_EQUAL(1, 2, "00", bm1);

    uchar data = 0x7F; // 0b01111111
    BitMask bm2(1, 4, &data);
    CPPUNIT_ASSERT_EQUAL(Size(1, 4), bm2.size());
    CPPUNIT_ASSERT_EQUAL(1, bm2.width());
    CPPUNIT_ASSERT_EQUAL(4, bm2.height());
    MASK_EQUAL(1, 4, "0111", bm2);
}

void TestBitMask::testInitWithString()
{
    BitMask bm(3, 3, "010101010");
    CPPUNIT_ASSERT_EQUAL(std::string("010\n101\n010\n"), toString(bm));

    // bad data
    bm.set("some data");
    CPPUNIT_ASSERT_EQUAL(std::string("010\n101\n010\n"), toString(bm));

    // empty data
    bm.set("");
    CPPUNIT_ASSERT_EQUAL(std::string("010\n101\n010\n"), toString(bm));

    // small init
    bm.set("000111");
    CPPUNIT_ASSERT_EQUAL(std::string("000\n111\n010\n"), toString(bm));

    // big init
    bm.set("000111000111000111000111");
    CPPUNIT_ASSERT_EQUAL(std::string("000\n111\n000\n"), toString(bm));
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
    CPPUNIT_ASSERT_EQUAL(std::string("000\n000\n000\n"), toString(bm));
    bm.invert();
    CPPUNIT_ASSERT_EQUAL(std::string("111\n111\n111\n"), toString(bm));
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

void TestBitMask::testCompare()
{
    BitMask bm1(2, 2, "1111");
    BitMask bm2(2, 3, "0000");
    CPPUNIT_ASSERT(bm1 != bm2);

    BitMask bm3(2, 3, "000000");
    CPPUNIT_ASSERT_EQUAL(bm2, bm3);
    bm2.set(0, 0);
    CPPUNIT_ASSERT(bm2 != bm3);

    BitMask bm4(9, 1);
    BitMask bm5(9, 1);

    bm4.fill(true);
    bm5.fill(true);
    CPPUNIT_ASSERT(bm4 == bm5);

    bm5.fill(false);
    CPPUNIT_ASSERT(bm4 != bm5);

    bm4.fill(true);
    bm5.set("111111111");
    CPPUNIT_ASSERT(bm4 == bm5);
    bm4.unset(0, 0);
    CPPUNIT_ASSERT(bm4 != bm5);
    bm5.unset(0, 0);
    CPPUNIT_ASSERT(bm4 == bm5);
    bm5.unset(1, 0);
    CPPUNIT_ASSERT(bm4 != bm5);
    bm4.unset(1, 0);
    CPPUNIT_ASSERT(bm4 == bm5);
    bm5.unset(2, 0);
    CPPUNIT_ASSERT(bm4 != bm5);
    bm4.unset(2, 0);
    CPPUNIT_ASSERT(bm4 == bm5);
    bm5.unset(3, 0);
    CPPUNIT_ASSERT(bm4 != bm5);
    bm4.unset(3, 0);
    CPPUNIT_ASSERT(bm4 == bm5);

    bm5.set("111111111");
    bm5.unset(0, 0);
    MASK_EQUAL(9, 1, "011111111", bm5);
    bm5.unset(1, 0);
    MASK_EQUAL(9, 1, "001111111", bm5);
    bm5.unset(2, 0);
    MASK_EQUAL(9, 1, "000111111", bm5);
    bm5.unset(3, 0);
    MASK_EQUAL(9, 1, "000011111", bm5);
    bm5.unset(4, 0);
    MASK_EQUAL(9, 1, "000001111", bm5);
    bm5.unset(5, 0);
    MASK_EQUAL(9, 1, "000000111", bm5);
    bm5.unset(6, 0);
    MASK_EQUAL(9, 1, "000000011", bm5);
    bm5.unset(7, 0);
    MASK_EQUAL(9, 1, "000000001", bm5);
    bm5.unset(8, 0);
    MASK_EQUAL(9, 1, "000000000", bm5);
}

void TestBitMask::testCopy()
{
    BitMask m1(1, 5);
    m1.set("00011");

    {
        BitMask m2(20, 40);
        m2.fill(true);
        m2 = m2;

        m2 = m1;
        CPPUNIT_ASSERT_EQUAL(m1, m2);
        MASK_EQUAL(1, 5, "00011", m2);
    }

    MASK_EQUAL(1, 5, "00011", m1);

    BitMask m3(m1);
    MASK_EQUAL(1, 5, "00011", m3);
}
