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

#include "testpicture.h"
#include "cpage/picture.h"
#include "cpage/block.h"
#include "cpage/cpage.h"
#include "cpage/polyblock.h"
#include "cpage/convert.h"
#include "common/tostring.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPicture);

using namespace cf;
using namespace cf::cpage;

void TestPicture::testInit()
{
    Picture p;
    CPPUNIT_ASSERT(p.cornerCount() == 0);
    CPPUNIT_ASSERT(p.cornerAt(0).x() == 0);
}

void TestPicture::testAppend()
{
    Picture p;
    CPPUNIT_ASSERT(p.cornerCount() == 0);
    p.appendCorner(Point(1, 1));
    CPPUNIT_ASSERT(p.cornerCount() == 1);
    CPPUNIT_ASSERT_EQUAL(Point(1, 1), p.cornerAt(0));

    p.appendCorner(Point(2, 2));
    CPPUNIT_ASSERT(p.cornerCount() == 2);
    CPPUNIT_ASSERT_EQUAL(Point(2, 2), p.cornerAt(1));
}

void TestPicture::testRotate()
{
    Picture p;
    p.appendCorner(Point(100, 100));
    p.appendCorner(Point(200, 200));
    p.rotateCorner(0, 2048);
    p.rotateCorner(1, 4096);

    CPPUNIT_ASSERT_EQUAL(Point(100, 0), p.cornerAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(-200, -200), p.cornerAt(1));

    p.clear();
    p.appendCorner(Point(0, 0));
    p.appendCorner(Point(100, 0));
    p.appendCorner(Point(100, 100));
    p.appendCorner(Point(0, 100));

    p.rotate(2048 / 90 * 5); // 5 degries
    CPPUNIT_ASSERT_EQUAL(Point(0, 0), p.cornerAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(100, -5), p.cornerAt(1));
    CPPUNIT_ASSERT_EQUAL(Point(106, 95), p.cornerAt(2));
    CPPUNIT_ASSERT_EQUAL(Point(6, 100), p.cornerAt(3));
}

void TestPicture::testConvert()
{
    Block b;
    PolyBlock poly;
    poly.addVertex(Point(1, 1));
    poly.addVertex(Point(2, 2));
    b.setData(TYPE_IMAGE, &poly, sizeof(PolyBlock));

    DefConvertInit();
    Block::setConvertor(&DefConvertBlock);
    CPPUNIT_ASSERT(b.Convert(0, NULL, 0) == 0);

    Picture p;
    // invalid type
    CPPUNIT_ASSERT(!b.Convert(TYPE_CPAGE_TABLE, &p, sizeof(Picture)));
    CPPUNIT_ASSERT(!p.cornerCount());

    // invalid dest size
    CPPUNIT_ASSERT(!b.Convert(TYPE_CPAGE_PICTURE, &p, sizeof(Picture) - 1));
    CPPUNIT_ASSERT(!p.cornerCount());

    // get only dest size
    CPPUNIT_ASSERT(b.Convert(TYPE_CPAGE_PICTURE, NULL, sizeof(Picture)) == sizeof(Picture));
    CPPUNIT_ASSERT(!p.cornerCount());

    // convert
    CPPUNIT_ASSERT_EQUAL(sizeof(Picture), (size_t) b.Convert(TYPE_CPAGE_PICTURE, &p, sizeof(Picture)));
    CPPUNIT_ASSERT(p.cornerCount() == 2);
    CPPUNIT_ASSERT_EQUAL(Point(1, 1), p.cornerAt(0));
    CPPUNIT_ASSERT_EQUAL(Point(2, 2), p.cornerAt(1));
}
