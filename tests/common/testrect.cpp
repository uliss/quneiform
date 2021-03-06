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
#include "testrect.h"
#include "../test_common.h"
#include <common/rect.h>
#include <common/tostring.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestRect);

using namespace cf;

void TestRect::testInit() {
    Rect r;
    CPPUNIT_ASSERT(!r.isNull());
    CPPUNIT_ASSERT_EQUAL(r.x(), 0);
    CPPUNIT_ASSERT_EQUAL(r.y(), 0);
    CPPUNIT_ASSERT_EQUAL(r.width(), 0);
    CPPUNIT_ASSERT_EQUAL(r.height(), 0);
    CPPUNIT_ASSERT_EQUAL(r.top(), 0);
    CPPUNIT_ASSERT_EQUAL(r.left(), 0);
    CPPUNIT_ASSERT_EQUAL(r.right(), 0);
    CPPUNIT_ASSERT_EQUAL(r.bottom(), 0);
    const Point p;
    CPPUNIT_ASSERT_EQUAL(r.center(), p);
    CPPUNIT_ASSERT_EQUAL(r.centerX(), 0);
    CPPUNIT_ASSERT_EQUAL(r.centerY(), 0);
    CPPUNIT_ASSERT_EQUAL(r.perimeter(), 0);
    //    CPPUNIT_ASSERT_EQUAL(r.size(), Size());
    CPPUNIT_ASSERT_EQUAL(r.pt0(), p);
    CPPUNIT_ASSERT_EQUAL(r.pt1(), p);
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

    r1.rleft() += 1;
    r1.rtop() += 1;
    r1.rright() += 1;
    r1.rbottom() += 1;
    CPPUNIT_ASSERT_EQUAL(r1.left(), 11);
    CPPUNIT_ASSERT_EQUAL(r1.top(), 16);
    CPPUNIT_ASSERT_EQUAL(r1.right(), 41);
    CPPUNIT_ASSERT_EQUAL(r1.bottom(), 56);

}

void TestRect::testSerialize() {
#ifdef CF_SERIALIZE
    const Rect r(Point(1, -2), 100, 200);
    const char * TXT = "serialize_rect.txt";

    writeToTextArchive(TXT, r);

    Rect new_r;
    CPPUNIT_ASSERT(r != new_r);
    readFromTextArchive(TXT, new_r);
    CPPUNIT_ASSERT_EQUAL(r, new_r);
#endif
}

void TestRect::testSerializeXml() {
#ifdef CF_SERIALIZE
    const Rect r(Point(1, -2), 100, 200);
    const char * XML = "serialize_rect.xml";

    writeToXmlArchive(XML, "rect", r);

    Rect new_r;
    CPPUNIT_ASSERT(r != new_r);
    readFromXmlArchive(XML, "rect", new_r);
    CPPUNIT_ASSERT_EQUAL(r, new_r);
#endif
}

void TestRect::testUnite() {
    Rect r1(Point(0, 0), Point(40, 40));

    //inside
    Rect r2(Point(10, 10), Point(30, 30));
    CPPUNIT_ASSERT(r1.contains(r2));
    CPPUNIT_ASSERT_EQUAL(r1, r1.united(r2));
    CPPUNIT_ASSERT_EQUAL(r1, r2.united(r1));
    //contains
    r2.set(Point(-10, -20), Point(50, 60));
    CPPUNIT_ASSERT(r2.contains(r1));
    CPPUNIT_ASSERT_EQUAL(r2, r1.united(r2));
    CPPUNIT_ASSERT_EQUAL(r2, r2.united(r1));
    // intersects
    r2.set(Point(20, 30), Point(50, 60));
    CPPUNIT_ASSERT(r2.intersects(r1));
    Point pt;
    Rect r_u1(pt, Point(50, 60));
    CPPUNIT_ASSERT_EQUAL(r_u1, r1.united(r2));
    // non intersects
    r2.set(Point(100, 100), Point(300, 300));
    CPPUNIT_ASSERT(!r2.intersects(r1));

    CPPUNIT_ASSERT_EQUAL(Rect(pt, Point(300, 300)), r2.united(r1));
}

void TestRect::testNormalize() {
    Rect r1(Point(0, 10), Point(30, 40));
    CPPUNIT_ASSERT(r1.isValid());
    Rect r2;
    CPPUNIT_ASSERT(r2.isValid());
    r2.setLeftTop(10, 20);
    CPPUNIT_ASSERT(!r2.isValid());
    r1.setRightBottom(-10, 40);
    CPPUNIT_ASSERT(!r1.isValid());
    r1.normalize();
    CPPUNIT_ASSERT_EQUAL(Rect(Point(-10, 10), Point(0, 40)), r1);
    CPPUNIT_ASSERT_EQUAL(r1, r1.normalized());
}

void TestRect::testHeight() {
    Rect r;
    r.setHeight(100);
    CPPUNIT_ASSERT_EQUAL(100, r.height());
    CPPUNIT_ASSERT_EQUAL(0, r.width());
    CPPUNIT_ASSERT(r.leftTop() == Point(0, 0));
    CPPUNIT_ASSERT(r.rightBottom() == Point(0, 100));
    r.setHeight(0);
    CPPUNIT_ASSERT(r.isValid());
    r.setHeight(-100);
    CPPUNIT_ASSERT(!r.isValid());
}

void TestRect::testWidth() {
    Rect r;
    r.setWidth(100);
    CPPUNIT_ASSERT_EQUAL(100, r.width());
    CPPUNIT_ASSERT_EQUAL(0, r.height());
    r.setWidth(0);
    CPPUNIT_ASSERT(r.isValid());
    r.setWidth(-100);
    CPPUNIT_ASSERT(!r.isValid());
}

void TestRect::testNull()
{
    CPPUNIT_ASSERT(Rect::null().isNull());
    CPPUNIT_ASSERT(!Rect::null().isValid());
}

void TestRect::testMoveXTo()
{
    Rect r(0, 0, 20, 30);
    r.moveXTo(10);
    CPPUNIT_ASSERT_EQUAL(Rect(10, 0, 20, 30), r);
    r.moveXTo(-10);
    CPPUNIT_ASSERT_EQUAL(Rect(-10, 0, 20, 30), r);
}

void TestRect::testMoveYTo()
{
    Rect r(0, 0, 20, 30);
    r.moveYTo(10);
    CPPUNIT_ASSERT_EQUAL(Rect(0, 10, 20, 30), r);
    r.moveYTo(-10);
    CPPUNIT_ASSERT_EQUAL(Rect(0, -10, 20, 30), r);
}

void TestRect::testMoveTo()
{
    Rect r(0, 0, 20, 30);
    r.moveTo(1, 2);
    CPPUNIT_ASSERT_EQUAL(Rect(1, 2, 20, 30), r);
    r.moveTo(-1, -2);
    CPPUNIT_ASSERT_EQUAL(Rect(-1, -2, 20, 30), r);

}

void TestRect::testIntersects()
{
    Rect r0(0, 0, 20, 30);
    Rect r1(50, 50, 20, 30);
    CPPUNIT_ASSERT(!r0.intersects(r1));
}

void TestRect::testIntersected()
{
    Rect r0(0, 0, 100, 200);
    CPPUNIT_ASSERT_EQUAL(Rect(1, 2, 30, 40), r0.intersected(Rect(1, 2, 30, 40)));
}
