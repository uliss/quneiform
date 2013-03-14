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
#include "common/log.h"
#include "common/bitmask.h"

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

static void printMatrix(const char * m, int x, int y)
{
    for(int i = 0; i < x * y; i++) {
        char ch = m[i];
        for(int k = 7; k >= 0; k--) {
            if(ch & (1 << k))
                std::cout << "1";
            else
                std::cout << "0";
        }

        if(i % x == (x-1))
            std::cout << std::endl;
    }
}

void TestCPicture::testGetMask()
{
    cf::Logger::config().enableLog(MODULE_CPAGE, MSG_TRACE);

    h_page = CPAGE_CreatePage(CPAGE_GetInternalType("test_picture"), NULL, 0);
    CBlockHandle pict = CPAGE_CreateBlock(h_page, TYPE_CPAGE_PICTURE, 3, 0, NULL, 0);

    BitMask mask;
    cpage::Picture pict_data;
    pict_data.appendCorner(Point(0, 0));
    CPAGE_SetBlockData(pict, TYPE_CPAGE_PICTURE, &pict_data, sizeof(pict_data));
    CPPUNIT_ASSERT(!CPAGE_PictureGetMask(pict, mask));
    pict_data.appendCorner(Point(10, 0));
    pict_data.appendCorner(Point(10, 20));
    pict_data.appendCorner(Point(0, 20));
    CPAGE_SetBlockData(pict, TYPE_CPAGE_PICTURE, &pict_data, sizeof(pict_data));

    CPPUNIT_ASSERT(CPAGE_PictureGetMask(pict, mask));
    CPPUNIT_ASSERT_EQUAL(Size(10, 20), mask.size());

    CPPUNIT_ASSERT(CPAGE_PictureGetMask(pict, mask));

    //   0 1 2 3 4 5 6 7 8 9 10
    // 0 * - - - - - - - - - *
    // 1 | . . . . . . . . . |
    // 2 | . * - - - - - - - *
    // 3 | . | . . . . . . . .
    // 4 | . | . . . . . . . .
    // 5 | . | . . . . . . . .
    // 6 | . * - - - - - - - *
    // 7 | . . . . . . . . . |
    // 8 | . . . * - - * . . |
    // 9 | . . . | . . | . . |
    //10 * - - - * . . * - - *

    pict_data.clear();
    pict_data.appendCorner(0, 0);
    pict_data.appendCorner(10, 0);
    pict_data.appendCorner(10, 10);
    pict_data.appendCorner(10, 2);
    pict_data.appendCorner(2, 2);
    pict_data.appendCorner(2, 6);
    pict_data.appendCorner(10, 6);
    pict_data.appendCorner(10, 10);
    pict_data.appendCorner(7, 10);
    pict_data.appendCorner(7, 8);
    pict_data.appendCorner(4, 8);
    pict_data.appendCorner(4, 10);
    pict_data.appendCorner(0, 10);

    CPAGE_SetBlockData(pict, TYPE_CPAGE_PICTURE, &pict_data, sizeof(pict_data));

    CPPUNIT_ASSERT(CPAGE_PictureGetMask(pict, mask));
    CPPUNIT_ASSERT_EQUAL(Size(10, 10), mask.size());

    BitMask mask2(10, 10, "1111111111"
                          "1111111111"
                          "1111111111"
                          "1110000000"
                          "1110000000"
                          "1110000000"
                          "1111111111"
                          "1111111111"
                          "1111111111"
                          "1111100111");
    CPPUNIT_ASSERT_EQUAL(mask, mask2);

    CPPUNIT_ASSERT(!CPAGE_PictureGetMask(NULL, mask));
}

void TestCPicture::tearDown()
{
    if(h_page) {
        CPAGE_DeletePage(h_page);
        h_page = NULL;
    }
}
