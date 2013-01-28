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

#include "testconvert.h"
#include "cpage/convert.h"
#include "cpage/picture.h"
#include "cpage/polyblock.h"
#include "cpage/cpage.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestConvert);

using namespace cf;
using namespace cpage;

void TestConvert::testPicture()
{
    Picture pict;
    pict.appendCorner(Point(0, 0));
    pict.appendCorner(Point(100, 0));
    pict.appendCorner(Point(100, 20));
    pict.appendCorner(Point(0, 20));
    PolyBlock poly;

    CPPUNIT_ASSERT_EQUAL(sizeof(PolyBlock), convertPicture(pict, 0, NULL, 0));
    CPPUNIT_ASSERT_EQUAL(size_t(0), convertPicture(pict, 0, &poly, 0));
    CPPUNIT_ASSERT_EQUAL(size_t(0), convertPicture(pict, 0, &poly, sizeof(PolyBlock)));

    CPPUNIT_ASSERT_EQUAL(sizeof(PolyBlock), convertPicture(pict, sizeof(Picture), &poly, sizeof(PolyBlock)));
    CPPUNIT_ASSERT_EQUAL(0, poly.number());
    CPPUNIT_ASSERT_EQUAL(pict.cornerCount(), poly.vertexCount());
    CPPUNIT_ASSERT_EQUAL(TYPE_PICTURE, poly.type());
}
