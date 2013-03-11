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

CPPUNIT_TEST_SUITE_REGISTRATION(TestBitMask);

using namespace cf;

void TestBitMask::testInit()
{
    BitMask bm(Size(2, 2));
    CPPUNIT_ASSERT(!bm.isSet(0, 0));
    CPPUNIT_ASSERT(!bm.isSet(1, 0));
    CPPUNIT_ASSERT(!bm.isSet(0, 1));
    CPPUNIT_ASSERT(!bm.isSet(0, 1));

    BitMask bm1(1, 2);
    CPPUNIT_ASSERT(!bm1.isSet(0, 0));
    CPPUNIT_ASSERT(!bm1.isSet(1, 1));

    uchar data = 0x7F; // 0b01111111
    BitMask bm2(1, 4, &data);
    CPPUNIT_ASSERT(!bm2.isSet(0, 0));
    CPPUNIT_ASSERT(bm2.isSet(0, 1));
    CPPUNIT_ASSERT(bm2.isSet(0, 2));
    CPPUNIT_ASSERT(bm2.isSet(0, 3));

}
