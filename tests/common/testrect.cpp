/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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
#include "testrect.h"
#include <rect.h>
#include <common/tostring.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestRect);

using namespace CIF;

void TestRect::testInit() {
    Rect r;
    CPPUNIT_ASSERT_EQUAL(r.x(), 0);
    CPPUNIT_ASSERT_EQUAL(r.y(), 0);
    CPPUNIT_ASSERT_EQUAL(r.width(), 0);
    CPPUNIT_ASSERT_EQUAL(r.height(), 0);
    CPPUNIT_ASSERT_EQUAL(r.top(), 0);
    CPPUNIT_ASSERT_EQUAL(r.left(), 0);
    CPPUNIT_ASSERT_EQUAL(r.right(), 0);
    CPPUNIT_ASSERT_EQUAL(r.bottom(), 0);
    CPPUNIT_ASSERT_EQUAL(r.center(), Point());
    CPPUNIT_ASSERT_EQUAL(r.centerX(), 0);
    CPPUNIT_ASSERT_EQUAL(r.centerY(), 0);
    CPPUNIT_ASSERT_EQUAL(r.perimeter(), 0);
    //    CPPUNIT_ASSERT_EQUAL(r.size(), Size());
    CPPUNIT_ASSERT_EQUAL(r.pt0(), Point());
    CPPUNIT_ASSERT_EQUAL(r.pt1(), Point());
    CPPUNIT_ASSERT_EQUAL(r, r);

    Rect r1(Point(10, 15), 30, 40);
    CPPUNIT_ASSERT_EQUAL(r1.width(), 30);
    CPPUNIT_ASSERT_EQUAL(r1.height(), 40);
    CPPUNIT_ASSERT_EQUAL(r1.x(), 10);
    CPPUNIT_ASSERT_EQUAL(r1.y(), 15);
    CPPUNIT_ASSERT_EQUAL(r1.top(), 15);
    CPPUNIT_ASSERT_EQUAL(r1.left(), 10);
    CPPUNIT_ASSERT_EQUAL(r1.right(), 40);
    CPPUNIT_ASSERT_EQUAL(r1.bottom(), 55);
    CPPUNIT_ASSERT_EQUAL(r1.center(), Point(25, 35));
    CPPUNIT_ASSERT_EQUAL(r1.centerX(), 25);
    CPPUNIT_ASSERT_EQUAL(r1.centerY(), 35);
    CPPUNIT_ASSERT_EQUAL(r1.perimeter(), 140);
    CPPUNIT_ASSERT_EQUAL(r1.pt0(), Point(10,15));
    CPPUNIT_ASSERT_EQUAL(r1.pt1(), Point(40, 55));
    CPPUNIT_ASSERT(r != r1);

    CPPUNIT_ASSERT_EQUAL(r1.leftTop(), Point(10, 15));
    CPPUNIT_ASSERT_EQUAL(r1.rightTop(), Point(40, 15));
    CPPUNIT_ASSERT_EQUAL(r1.leftBottom(), Point(10, 55));
    CPPUNIT_ASSERT_EQUAL(r1.rightBottom(), Point(40, 55));

}