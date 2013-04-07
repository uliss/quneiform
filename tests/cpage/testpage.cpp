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

#include <sstream>
#include <fstream>
#include <string.h>

#include "testpage.h"
#include "cpage/page.h"
#include "cpage/block.h"
#include "cpage/convert.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestPage);

using namespace cf::cpage;

void TestPage::testInit()
{
    Page p;
    CPPUNIT_ASSERT(p.empty());
    CPPUNIT_ASSERT_EQUAL(size_t(0), p.blockCount());
    CPPUNIT_ASSERT(!p.blockAt(0));
    CPPUNIT_ASSERT(!((const Page*)&p)->blockAt(0));
    CPPUNIT_ASSERT_EQUAL(int(-1), p.findBlock(NULL));
    CPPUNIT_ASSERT(!p.removeBlock(NULL));

    Block * b = p.createBlock(0);
    CPPUNIT_ASSERT(p.blockCount() == 1);
    CPPUNIT_ASSERT(b);
    CPPUNIT_ASSERT_EQUAL(b, p.blockAt(0));
}

void TestPage::appendBlock()
{
    Page p;
    CPPUNIT_ASSERT_EQUAL(size_t(0), p.blockCount());

    Block b;
    b.setFlags(10);
    p.appendBlock(b);
    CPPUNIT_ASSERT_EQUAL(size_t(1), p.blockCount());
    Block * last = p.blockAt(0);
    CPPUNIT_ASSERT_EQUAL(int(0), p.findBlock(last));

    CPPUNIT_ASSERT(p.blockAt(0)->empty());
    CPPUNIT_ASSERT_EQUAL(p.blockAt(0)->flags(), b.flags());

    CPPUNIT_ASSERT(!p.removeBlock(NULL));
    CPPUNIT_ASSERT(p.removeBlock(last));
    CPPUNIT_ASSERT_EQUAL(size_t(0), p.blockCount());
    p.clearBlocks();
}

void TestPage::testSaveRestore()
{
    Page p;
    p.setType(CPAGE_GetInternalType("test_save"));
    p.createBlock(CPAGE_GetInternalType("test_save"), 1);
    p.createBlock(CPAGE_GetInternalType("test_save"), 2);

    std::ofstream bad;
    bad << 0xDEADBEEF;
    CPPUNIT_ASSERT(!p.save(bad));

    std::stringstream buf;
    CPPUNIT_ASSERT(p.save(buf));

    {
        Page p2;
        std::ifstream bad;
        int tmp;
        bad >> tmp;
        CPPUNIT_ASSERT(!p.restore(bad));

        CPPUNIT_ASSERT(p2.restore(buf));

        CPPUNIT_ASSERT_EQUAL(size_t(2), p2.blockCount());
        CPPUNIT_ASSERT_EQUAL(uint32_t(1), p2.blockAt(0)->userNum());
        CPPUNIT_ASSERT_EQUAL(uint32_t(2), p2.blockAt(1)->userNum());
    }
}

void TestPage::testCopy()
{
    uchar data[10];
    memset(data, 0xff, sizeof(data));
    Page p;
    p.setData(0, data, sizeof(data));
    p.createBlock(0, 1, 0, data, 6);
    p.createBlock(0, 2, 0, data, 4);

    Page p2(p);
    CPPUNIT_ASSERT(p2.blockCount() == 2);

    Page p3;
    CPPUNIT_ASSERT(p3.blockCount() == 0);
    p3 = p;
    CPPUNIT_ASSERT(p2.blockCount() == 2);
}

void TestPage::testPageInfo()
{
    Page p;
    CPPUNIT_ASSERT(!p.pageInfo());

    PAGEINFO pi;
    p.setData(PT_PAGEINFO, &pi, 0);
    CPPUNIT_ASSERT(!p.pageInfo());

    p.setData(PT_PAGEINFO, &pi, sizeof(PAGEINFO));
    CPPUNIT_ASSERT(p.pageInfo());
}

static uint32_t test_convert(uint32_t /*dwContext*/,
                             CDataType /*TypeIn*/,  const void * /*lpDataIn*/,  uint32_t /*SizeIn*/,
                             CDataType /*TypeOut*/, void * /*LpDataOut*/, uint32_t SizeOut)
{
    return SizeOut * 11;
}

void TestPage::testConvertor()
{
    DataConvertor cv(&test_convert);
    CPPUNIT_ASSERT_EQUAL(&DefConvertPage, Page::setConvertor(cv).func());

    uchar data[10];
    Page p;
    CDataType first_type = CPAGE_GetInternalType("test_convert");
    CDataType second_type = CPAGE_GetInternalType("other type");
    CPPUNIT_ASSERT(first_type != second_type);

    p.setData(first_type, data, sizeof(data));
    CPPUNIT_ASSERT_EQUAL(uint32_t(121), p.getData(second_type, NULL, 11));
}
