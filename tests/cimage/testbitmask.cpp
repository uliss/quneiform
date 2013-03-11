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

#include "testbitmask.h"
#include "common/bitmask.h"
#include "test_cimage_common.h"
#include "cimage/cticontrol.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestBitMask);

using namespace cf;

#define CHECK_Y(m, x, y) CPPUNIT_ASSERT(m.isSet(x, y))
#define CHECK_N(m, x, y) CPPUNIT_ASSERT(!(m.isSet(x, y)))

void TestBitMask::testInit()
{
    BitMask mask(10, 10);
    for(int i = 0; i < 10; i++) {
        for(int j = 0; j < 10; j++) {
            CHECK_N(mask, i, j);
        }
    }

    BitMask mask2(Size(1, 1));
    CHECK_N(mask2, 0, 0);
}

void TestBitMask::testFill()
{
    BitMask mask(2, 1);
    mask.fill(true);

    CHECK_Y(mask, 0, 0);
    CHECK_Y(mask, 1, 0);

    mask.fill(false);
    CHECK_N(mask, 0, 0);
    CHECK_N(mask, 1, 0);
}

void TestBitMask::testInvert()
{
    BitMask mask(2, 2);

    CHECK_N(mask, 0, 0);
    CHECK_N(mask, 1, 0);
    CHECK_N(mask, 0, 1);
    CHECK_N(mask, 1, 1);

    mask.invert();

    CHECK_Y(mask, 0, 0);
    CHECK_Y(mask, 1, 0);
    CHECK_Y(mask, 0, 1);
    CHECK_Y(mask, 1, 1);

    mask.unset(0, 0);
    CHECK_N(mask, 0, 0);

    mask.invert();
    CHECK_Y(mask, 0, 0);
    CHECK_N(mask, 1, 0);
    CHECK_N(mask, 0, 1);
    CHECK_N(mask, 1, 1);
}

void TestBitMask::testFillRect()
{
    BitMask m(4, 4);
    m.fillRect(Rect(Point(1, 1), 2, 2), true);

    CHECK_N(m, 0, 0);
    CHECK_N(m, 0, 1);
    CHECK_N(m, 0, 2);
    CHECK_N(m, 0, 3);

    CHECK_N(m, 1, 0);
    CHECK_Y(m, 1, 1);
    CHECK_Y(m, 1, 2);
    CHECK_N(m, 1, 3);

    CHECK_N(m, 2, 0);
    CHECK_Y(m, 2, 1);
    CHECK_Y(m, 2, 2);
    CHECK_N(m, 2, 3);

    CHECK_N(m, 3, 0);
    CHECK_N(m, 3, 1);
    CHECK_N(m, 3, 2);
    CHECK_N(m, 3, 3);
}

void TestBitMask::testSet()
{
    BitMask m(2, 2);
    CHECK_N(m, 0, 0);
    CHECK_N(m, 1, 0);
    CHECK_N(m, 0, 1);
    CHECK_N(m, 1, 1);

    m.set(0, 1);
    CHECK_N(m, 0, 0);
    CHECK_Y(m, 0, 1);
    CHECK_N(m, 1, 0);
    CHECK_N(m, 1, 1);

    m.set(1, 0);
    CHECK_N(m, 0, 0);
    CHECK_Y(m, 0, 1);
    CHECK_Y(m, 1, 0);
    CHECK_N(m, 1, 1);
}

void TestBitMask::testUnset()
{
    BitMask m(2, 2);
    m.invert();
    CHECK_Y(m, 0, 0);
    CHECK_Y(m, 1, 0);
    CHECK_Y(m, 0, 1);
    CHECK_Y(m, 1, 1);

    m.unset(0, 1);
    CHECK_Y(m, 0, 0);
    CHECK_N(m, 0, 1);
    CHECK_Y(m, 1, 0);
    CHECK_Y(m, 1, 1);

    m.unset(1, 0);
    CHECK_Y(m, 0, 0);
    CHECK_N(m, 0, 1);
    CHECK_N(m, 1, 0);
    CHECK_Y(m, 1, 1);
}

void TestBitMask::testApply1Bit()
{
    DibPtr dib("black_1.bmp");
    CPPUNIT_ASSERT(dib);
    BitMask mask(100, 100);
    mask.fillRect(Rect(Point(20, 20), 60, 60), true);
    mask.fillRect(Rect(Point(35, 35), 30, 30), false);

    CPPUNIT_ASSERT(dib->applyMask(mask));
    CTIControl::writeDIBtoBMP("cimage_bit_mask_apply_1.bmp", dib);
    IS_WHITE_DIB1(dib, 0, 0);
    IS_BLACK_DIB1(dib, 20, 20);
    IS_WHITE_DIB1(dib, 35, 35);
}

void TestBitMask::testApply32Bit()
{
    DibPtr dib("black_32.bmp");
    CPPUNIT_ASSERT(dib);
    BitMask mask(100, 100);
    mask.fillRect(Rect(Point(20, 20), 60, 60), true);
    mask.fillRect(Rect(Point(35, 35), 30, 30), false);

    CPPUNIT_ASSERT(dib->applyMask(mask));
    CTIControl::writeDIBtoBMP("cimage_bit_mask_apply_32.bmp", dib);
    IS_WHITE_DIB32(dib, 0, 0);
    IS_BLACK_DIB32(dib, 20, 20);
    IS_WHITE_DIB32(dib, 35, 35);
}

void TestBitMask::testApply8Bit()
{
    DibPtr dib("black_8.bmp");
    CPPUNIT_ASSERT(dib);
    BitMask mask(100, 100);
    mask.fillRect(Rect(Point(20, 20), 60, 60), true);
    mask.fillRect(Rect(Point(35, 35), 30, 30), false);

    CPPUNIT_ASSERT(dib->applyMask(mask));
    CTIControl::writeDIBtoBMP("cimage_bit_mask_apply_8.bmp", dib);

    DibPtr dib2("black_gray.bmp");
    CPPUNIT_ASSERT(dib2->applyMask(mask));
    CTIControl::writeDIBtoBMP("cimage_bit_mask_apply_gray.bmp", dib2);
    IS_WHITE_DIB1(dib2, 0, 0);
    IS_BLACK_DIB1(dib2, 20, 20);
    IS_WHITE_DIB1(dib2, 35, 35);
}

void TestBitMask::testApply24Bit()
{
    DibPtr dib("black_24.bmp");
    CPPUNIT_ASSERT(dib);
    BitMask mask(100, 100);
    mask.fillRect(Rect(Point(20, 20), 60, 60), true);
    mask.fillRect(Rect(Point(35, 35), 30, 30), false);

    CPPUNIT_ASSERT(dib->applyMask(mask));
    CTIControl::writeDIBtoBMP("cimage_bit_mask_apply_24.bmp", dib);
    IS_WHITE_DIB32(dib, 0, 0);
    IS_BLACK_DIB32(dib, 20, 20);
    IS_WHITE_DIB32(dib, 35, 35);
}
