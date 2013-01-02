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

#include "testcommondata.h"
#include "cpage/commondata.h"
#include "cpage/picture.h"
#include "common/tostring.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCommonData);

using namespace cf;
using namespace cf::cpage;

void TestCommonData::testInit()
{
    CommonData cd;
    CPPUNIT_ASSERT(!cd.flags());
    CPPUNIT_ASSERT(!cd.type());
    CPPUNIT_ASSERT(!cd.vertexCount());
    CPPUNIT_ASSERT(!cd.number());
    cd.setType(0);
}

void TestCommonData::testFlags()
{
    CommonData cd;
    cd.setFlag(0x1);
    cd.setFlag(0x2);
    cd.setFlag(0x4);
    CPPUNIT_ASSERT_EQUAL(0x7, (int) cd.flags());

    cd.setFlags(0x3);
    CPPUNIT_ASSERT_EQUAL(0x3, (int) cd.flags());
}


void TestCommonData::testNumber()
{
    CommonData cd;
    CPPUNIT_ASSERT(cd.number() == 0);
    cd.setNumber(10);
    CPPUNIT_ASSERT_EQUAL(10, (int) cd.number());
}

void TestCommonData::testCalcHeight()
{
    CommonData cd;
    CPPUNIT_ASSERT(!cd.calcHeight());
    cd.addVertex(0, 1);
    cd.addVertex(10, 1);
    cd.addVertex(4, 5);
    cd.addVertex(-1, -5);
    cd.addVertex(34, 2);
    CPPUNIT_ASSERT_EQUAL(10, cd.calcHeight());
}

void TestCommonData::testSetRect()
{
    CommonData cd;
    CPPUNIT_ASSERT(!cd.isRect());
    cd.setRect(Rect(1, 2, 100, 200));
    CPPUNIT_ASSERT_EQUAL(4, (int) cd.vertexCount());
    CPPUNIT_ASSERT_EQUAL(Rect(1, 2, 100, 200), cd.rect());
    CPPUNIT_ASSERT(cd.isRect());
}

void TestCommonData::testVertexes()
{
    CommonData cd;
    cd.addVertex(10, 20);
    CPPUNIT_ASSERT_EQUAL(1, (int) cd.vertexCount());
    cd.setVertex(0, Point(30, 40));
    CPPUNIT_ASSERT_EQUAL(Point(30, 40), cd.vertexAt(0));
    cd.setVertexX(0, 5);
    CPPUNIT_ASSERT_EQUAL(Point(5, 40), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(5, cd.vertexX(0));
    cd.setVertexY(0, 6);
    CPPUNIT_ASSERT_EQUAL(Point(5, 6), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(6, cd.vertexY(0));

    cd.moveVertexX(0, 1);
    CPPUNIT_ASSERT_EQUAL(Point(6, 6), cd.vertexAt(0));
    cd.moveVertexY(0, 1);
    CPPUNIT_ASSERT_EQUAL(Point(6, 7), cd.vertexAt(0));
}

void TestCommonData::testIsRect()
{
    CommonData cd;
    CPPUNIT_ASSERT(!cd.isRect());
    cd.addVertex(0, 0);
    cd.addVertex(10, 0);
    cd.addVertex(10, 10);
    cd.addVertex(0, 10);
    CPPUNIT_ASSERT(cd.isRect());

    cd.moveVertexX(0, 1);
    CPPUNIT_ASSERT(!cd.isRect());
    cd.moveVertexX(0, -1);
    CPPUNIT_ASSERT(cd.isRect());
    cd.moveVertexY(0, 1);
    CPPUNIT_ASSERT(!cd.isRect());
    cd.moveVertexY(0, -1);
    CPPUNIT_ASSERT(cd.isRect());

    cd.moveVertexX(1, 1);
    CPPUNIT_ASSERT(!cd.isRect());
    cd.moveVertexX(1, -1);
    CPPUNIT_ASSERT(cd.isRect());
    cd.moveVertexY(1, 1);
    CPPUNIT_ASSERT(!cd.isRect());
    cd.moveVertexY(1, -1);
    CPPUNIT_ASSERT(cd.isRect());

    cd.moveVertexX(2, 1);
    CPPUNIT_ASSERT(!cd.isRect());
    cd.moveVertexX(2, -1);
    CPPUNIT_ASSERT(cd.isRect());
    cd.moveVertexY(2, 1);
    CPPUNIT_ASSERT(!cd.isRect());
    cd.moveVertexY(2, -1);
    CPPUNIT_ASSERT(cd.isRect());

    cd.moveVertexX(3, 1);
    CPPUNIT_ASSERT(!cd.isRect());
    cd.moveVertexX(3, -1);
    CPPUNIT_ASSERT(cd.isRect());
    cd.moveVertexY(3, 1);
    CPPUNIT_ASSERT(!cd.isRect());
    cd.moveVertexY(3, -1);
    CPPUNIT_ASSERT(cd.isRect());

    cd.setRect(Rect());
    CPPUNIT_ASSERT(!cd.isRect());
    cd.setRect(Rect(0, 0, -10, 30));
    CPPUNIT_ASSERT(!cd.isRect());

    cd.setRect(Rect(0, 0, 10, -30));
    CPPUNIT_ASSERT(!cd.isRect());
}

void TestCommonData::testCopyVertexes()
{
    CommonData cd;
    CPPUNIT_ASSERT(!cd.vertexCount());

    Picture pict;
    cd.copyVertexes(pict);
    CPPUNIT_ASSERT(!cd.vertexCount());

    pict.appendCorner(Point(1, 2));
    pict.appendCorner(Point(3, 4));

    cd.copyVertexes(pict);
    CPPUNIT_ASSERT(cd.vertexCount() == 2);
    CPPUNIT_ASSERT_EQUAL(pict.cornerAt(0), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(pict.cornerAt(1), cd.vertexAt(1));
}

void TestCommonData::testInsertVertex()
{
    CommonData cd;
    cd.insertVertex(0, Point(1, 2));
    CPPUNIT_ASSERT(cd.vertexCount());

    cd.insertVertex(0, Point(3, 4));
    CPPUNIT_ASSERT_EQUAL(2, (int) cd.vertexCount());
    CPPUNIT_ASSERT_EQUAL(Point(3, 4), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(1, 2), cd.vertexAt(1));

    cd.insertVertex(0, Point(5, 6));
    CPPUNIT_ASSERT_EQUAL(3, (int) cd.vertexCount());
    CPPUNIT_ASSERT_EQUAL(Point(5, 6), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(3, 4), cd.vertexAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(1, 2), cd.vertexAt(2));

    cd.insertVertex(1, Point(7, 8));
    CPPUNIT_ASSERT_EQUAL(4, (int) cd.vertexCount());
    CPPUNIT_ASSERT_EQUAL(Point(5, 6), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(7, 8), cd.vertexAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(3, 4), cd.vertexAt(2));
    CPPUNIT_ASSERT_EQUAL(Point(1, 2), cd.vertexAt(3));

    cd.insertVertex(4, Point(9, 10));
    CPPUNIT_ASSERT_EQUAL(5, (int) cd.vertexCount());
    CPPUNIT_ASSERT_EQUAL(Point(5, 6), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(7, 8), cd.vertexAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(3, 4), cd.vertexAt(2));
    CPPUNIT_ASSERT_EQUAL(Point(1, 2), cd.vertexAt(3));
    CPPUNIT_ASSERT_EQUAL(Point(9, 10), cd.vertexAt(4));
}

void TestCommonData::testInsertBottom()
{
    CommonData cd;
    Rect r;

    cd.insertBottom(r);
    CPPUNIT_ASSERT(!cd.vertexCount());

    //@.........@
    cd.addVertex(0, 0);
    cd.addVertex(10, 0);
    r.set(Point(3, -5), 4, 6);
    cd.insertBottom(r);
    CPPUNIT_ASSERT_EQUAL(6, (int) cd.vertexCount());
    CPPUNIT_ASSERT_EQUAL(Point(0, 0), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(3, 0), cd.vertexAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(3, 1), cd.vertexAt(2));
    CPPUNIT_ASSERT_EQUAL(Point(7, 1), cd.vertexAt(3));
    CPPUNIT_ASSERT_EQUAL(Point(7, 0), cd.vertexAt(4));
    CPPUNIT_ASSERT_EQUAL(Point(10, 0), cd.vertexAt(5));

    // became:
    //
    //@**@...@**@
    //...@***@...
}

void TestCommonData::testInsertTop()
{
    CommonData cd;
    Rect r;
    cd.insertTop(r);
    CPPUNIT_ASSERT(!cd.vertexCount());

    cd.addVertex(10, 0);
    cd.addVertex(0, 0);
    // @........@

    r.set(Point(3, -1), 4, 6);
    cd.insertTop(r);
    //-1 ...@***@
    // 0 @**@...@**@

    CPPUNIT_ASSERT_EQUAL(6, (int) cd.vertexCount());
    CPPUNIT_ASSERT_EQUAL(Point(10, 0), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(7, 0), cd.vertexAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(7, -1), cd.vertexAt(2));
    CPPUNIT_ASSERT_EQUAL(Point(3, -1), cd.vertexAt(3));
    CPPUNIT_ASSERT_EQUAL(Point(3, 0), cd.vertexAt(4));
    CPPUNIT_ASSERT_EQUAL(Point(0, 0), cd.vertexAt(5));

}

void TestCommonData::testInsertLeft()
{
    CommonData cd;
    Rect r;
    cd.insertLeft(r);
    CPPUNIT_ASSERT(!cd.vertexCount());

    cd.addVertex(0, 0);
    cd.addVertex(0, 4);

    // @
    // .
    // .
    // .
    // @

    r.set(Point(-1, 1), Point(1, 3));
    cd.insertLeft(r);
    // .@
    // @@
    // *.
    // @@
    // .@

    CPPUNIT_ASSERT_EQUAL(6, (int) cd.vertexCount());
    CPPUNIT_ASSERT_EQUAL(Point(0, 0), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(0, 1), cd.vertexAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(-1, 1), cd.vertexAt(2));
    CPPUNIT_ASSERT_EQUAL(Point(-1, 3), cd.vertexAt(3));
    CPPUNIT_ASSERT_EQUAL(Point(0, 3), cd.vertexAt(4));
    CPPUNIT_ASSERT_EQUAL(Point(0, 4), cd.vertexAt(5));
}

void TestCommonData::testInsertRight()
{
    CommonData cd;
    Rect r;
    cd.insertRight(r);
    CPPUNIT_ASSERT(!cd.vertexCount());

    cd.addVertex(0, 4);
    cd.addVertex(0, 0);

    // @
    // .
    // .
    // .
    // @

    r.set(Point(-1, 1), Point(1, 3));
    cd.insertRight(r);
    // @
    // @@
    // .*
    // @@
    // @

    CPPUNIT_ASSERT_EQUAL(6, (int) cd.vertexCount());
    CPPUNIT_ASSERT_EQUAL(Point(0, 4), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(0, 3), cd.vertexAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(1, 3), cd.vertexAt(2));
    CPPUNIT_ASSERT_EQUAL(Point(1, 1), cd.vertexAt(3));
    CPPUNIT_ASSERT_EQUAL(Point(0, 1), cd.vertexAt(4));
    CPPUNIT_ASSERT_EQUAL(Point(0, 0), cd.vertexAt(5));

    cd.clearVertexes();
    cd.insertRight(r);
    CPPUNIT_ASSERT(!cd.vertexCount());

    cd.setRect(Rect(0, 0, 10, 4));
    CPPUNIT_ASSERT_EQUAL(4, (int) cd.vertexCount());

    // @********@
    // *........*
    // *........*
    // *........*
    // @********@

    r.set(Point(-1, 1), Point(1, 3));
    cd.insertRight(r);
    // @*********@
    // @@........*
    // .*........*
    // @@........*
    // @*********@

    CPPUNIT_ASSERT_EQUAL(8, (int) cd.vertexCount());
    CPPUNIT_ASSERT_EQUAL(Point(0, 0), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(10, 0), cd.vertexAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(10, 4), cd.vertexAt(2));
    CPPUNIT_ASSERT_EQUAL(Point(0, 4), cd.vertexAt(3));
    CPPUNIT_ASSERT_EQUAL(Point(0, 3), cd.vertexAt(4));
    CPPUNIT_ASSERT_EQUAL(Point(1, 3), cd.vertexAt(5));
    CPPUNIT_ASSERT_EQUAL(Point(1, 1), cd.vertexAt(6));
    CPPUNIT_ASSERT_EQUAL(Point(0, 1), cd.vertexAt(7));
}

void TestCommonData::testRotate()
{
    CommonData cd;
    cd.rotateVertexesToIdeal(2048);
    CPPUNIT_ASSERT(!cd.vertexCount());

    cd.setRect(Rect(0, 0, 100, 200));
    CPPUNIT_ASSERT_EQUAL(Point(0, 0), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(100, 0), cd.vertexAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(100, 200), cd.vertexAt(2));
    CPPUNIT_ASSERT_EQUAL(Point(0, 200), cd.vertexAt(3));

    cd.rotateVertexesToIdeal(1024);
    CPPUNIT_ASSERT_EQUAL(4, (int) cd.vertexCount());
    CPPUNIT_ASSERT_EQUAL(Point(0, 0), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(100, -50), cd.vertexAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(200, 100), cd.vertexAt(2));
    CPPUNIT_ASSERT_EQUAL(Point(100, 150), cd.vertexAt(3));
    CPPUNIT_ASSERT_EQUAL(Rect(0, 0, 200, 100), cd.rect().normalized());

    cd.rotateVertexesToReal(1024);
    CPPUNIT_ASSERT_EQUAL(Point(0, 0), cd.vertexAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(100, 0), cd.vertexAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(100, 200), cd.vertexAt(2));
    CPPUNIT_ASSERT_EQUAL(Point(0, 200), cd.vertexAt(3));
}
