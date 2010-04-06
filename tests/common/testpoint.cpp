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
#include <fstream>
#include "testpoint.h"
#include <common/point.h>
#include <common/tostring.h>
#include <ced/cedarchive.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestPoint);

using namespace CIF;

void TestPoint::testInit() {
    Point pt;
    CPPUNIT_ASSERT_EQUAL(pt.x(), 0);
    CPPUNIT_ASSERT_EQUAL(pt.y(), 0);

    CPPUNIT_ASSERT(!pt.isNegative());

    Point p1(10, 20);
    Point p2(-40, 50);
    Point p3 = p1 + p2;
    CPPUNIT_ASSERT_EQUAL(p3, Point(-30, 70));

    p3 = p2 - p1;
    CPPUNIT_ASSERT_EQUAL(p3, Point(-50, 30));

    p1.setX(100);
    CPPUNIT_ASSERT_EQUAL(p1, Point(100, 20));
    p1.setY(-300);
    CPPUNIT_ASSERT_EQUAL(p1, Point(100, -300));

    p1.rx()++;
    p1.ry()--;
    CPPUNIT_ASSERT_EQUAL(p1, Point(101, -301));

    CPPUNIT_ASSERT(p1 != p2);
    CPPUNIT_ASSERT(p1 == p1);

    p2 += p2;
    CPPUNIT_ASSERT_EQUAL(p2, Point(-80, 100));

    p3 -= p3;
    Point null_pt;
    CPPUNIT_ASSERT_EQUAL(p3, null_pt);

    CPPUNIT_ASSERT_EQUAL(PointXDistance(Point(20, 0), Point(30, 0)), 10);
    CPPUNIT_ASSERT_EQUAL(PointXDelta(Point(20, 0), Point(30, 0)), -10);
    CPPUNIT_ASSERT_EQUAL(PointYDistance(Point(0, 20), Point(30, 30)), 10);
    CPPUNIT_ASSERT_EQUAL(PointYDelta(Point(20, 20), Point(30, 30)), -10);

    Point p4 = leftmost(Point(200, 20), Point(100, 50));
    CPPUNIT_ASSERT_EQUAL(p4, Point(100, 50));
}

void TestPoint::testCompare() {
    Point p1(10, 20), p2(20, 30);
    CPPUNIT_ASSERT_EQUAL(highest(p2, p1), p1);
    CPPUNIT_ASSERT_EQUAL(lowest(p2, p1), p2);
    CPPUNIT_ASSERT_EQUAL(leftmost(p2, p1), p1);
    CPPUNIT_ASSERT_EQUAL(rightmost(p2, p1), p2);
}

void TestPoint::testOverflow() {
    Point p1(32000, 32000);
    Point16 p2;
    p2 = p1;
    //    p2 += 1000;
}

void TestPoint::testSerialize() {
#ifdef CF_SERIALIZE
    Point pt(1, 2);

    std::ofstream ofs("serialize_point.txt");

    // save data to archive
    {
        CEDOutputArchive oa(ofs);
        // write class instance to archive
        oa << pt;
    }

    Point new_point;
    CPPUNIT_ASSERT(pt != new_point);
    {
        // create and open an archive for input
        std::ifstream ifs("serialize_point.txt");
        CEDInputArchive ia(ifs);
        // read class state from archive
        ia >> new_point;
    }

    CPPUNIT_ASSERT_EQUAL(pt, new_point);

}
#endif

