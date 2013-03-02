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

#include "testcpicture.h"
#include "cpage/cpage.h"
#include "cpage/picture.h"
#include "common/tostring.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCPicture);

static CPageHandle h_page = NULL;
using namespace cf;

void TestCPicture::testGetPlace()
{
    h_page = CPAGE_CreatePage(CPAGE_GetInternalType("test_picture"), NULL, 0);
    CPPUNIT_ASSERT(h_page);
    CBlockHandle h_pict = CPAGE_CreateBlock(h_page, TYPE_CPAGE_PICTURE, 0, 0, NULL, 0);
    CPPUNIT_ASSERT(h_pict);
    bool rc = CPAGE_PictureGetPlace(h_pict, 0, NULL, NULL);
    CPPUNIT_ASSERT(!rc);

    Point pos;
    Size sz;

    rc = CPAGE_PictureGetPlace(h_pict, 0, &pos, &sz);
    CPPUNIT_ASSERT(!rc);

    cpage::Picture pdata;
    CPPUNIT_ASSERT(CPAGE_SetBlockData(h_pict, TYPE_CPAGE_PICTURE, &pdata, sizeof(pdata)));
    rc = CPAGE_PictureGetPlace(h_pict, 0, &pos, &sz);
    CPPUNIT_ASSERT(!rc);

    pdata.appendCorner(Point(0, 0));
    pdata.appendCorner(Point(0, 100));
    pdata.appendCorner(Point(100, 100));
    pdata.appendCorner(Point(100, 0));
    CPPUNIT_ASSERT(CPAGE_SetBlockData(h_pict, TYPE_CPAGE_PICTURE, &pdata, sizeof(pdata)));

    rc = CPAGE_PictureGetPlace(h_pict, 0, &pos, &sz);
    CPPUNIT_ASSERT(rc);
    CPPUNIT_ASSERT_EQUAL(pos, Point(0, 0));
    CPPUNIT_ASSERT_EQUAL(sz, Size(100, 100));

    // rotate 5 degrees counter clockwise
    rc = CPAGE_PictureGetPlace(h_pict, 2048 / 90 * 5, &pos, &sz);
    CPPUNIT_ASSERT(rc);
    CPPUNIT_ASSERT_EQUAL(pos, Point(0, -5));
    CPPUNIT_ASSERT_EQUAL(sz, Size(106, 105));
}

void TestCPicture::testGetFirstNext()
{
    h_page = CPAGE_CreatePage(CPAGE_GetInternalType("test_picture"), NULL, 0);
    CPAGE_CreateBlock(h_page, TYPE_TEXT, 1, 0, NULL, 0);
    CPAGE_CreateBlock(h_page, TYPE_TEXT, 2, 0, NULL, 0);
    CBlockHandle pict = CPAGE_CreateBlock(h_page, TYPE_CPAGE_PICTURE, 3, 0, NULL, 0);
    CPPUNIT_ASSERT(pict);

    CPPUNIT_ASSERT_EQUAL(pict, CPAGE_PictureGetFirst(h_page));
    CPPUNIT_ASSERT(!CPAGE_PictureGetNext(h_page, pict));
}

void TestCPicture::testGetMask()
{
    h_page = CPAGE_CreatePage(CPAGE_GetInternalType("test_picture"), NULL, 0);
    CBlockHandle pict = CPAGE_CreateBlock(h_page, TYPE_CPAGE_PICTURE, 3, 0, NULL, 0);

    CPPUNIT_ASSERT(!CPAGE_PictureGetMask(pict, NULL, NULL));

    uint32_t sz = 0;
    CPPUNIT_ASSERT(!CPAGE_PictureGetMask(pict, NULL, &sz));

    cpage::Picture pict_data;
    pict_data.appendCorner(Point(0, 0));
    CPAGE_SetBlockData(pict, TYPE_CPAGE_PICTURE, &pict_data, sizeof(pict_data));
    CPPUNIT_ASSERT(!CPAGE_PictureGetMask(pict, NULL, &sz));
    pict_data.appendCorner(Point(10, 5));
    pict_data.appendCorner(Point(12, 5));
    pict_data.appendCorner(Point(12, 0));
    pict_data.appendCorner(Point(20, 0));
    pict_data.appendCorner(Point(20, 50));
    pict_data.appendCorner(Point(0, 50));
    pict_data.appendCorner(Point(0, 50));
    CPAGE_SetBlockData(pict, TYPE_CPAGE_PICTURE, &pict_data, sizeof(pict_data));
    CPPUNIT_ASSERT(CPAGE_PictureGetMask(pict, NULL, &sz));
    CPPUNIT_ASSERT_EQUAL(150, (int) sz);

    char * matrix = new char[sz];
    CPPUNIT_ASSERT(CPAGE_PictureGetMask(pict, matrix, &sz));
    for(int i = 0; i < 150; i++) {
        std::cout << std::hex << (uint) (uchar) (matrix[i]) << " ";
    }
    delete[] matrix;

    std::cout << std::dec;
}

void TestCPicture::tearDown()
{
    if(h_page) {
        CPAGE_DeletePage(h_page);
        h_page = NULL;
    }
}
