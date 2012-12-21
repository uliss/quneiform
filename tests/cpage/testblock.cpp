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

#include <string.h>
#include <fstream>

#include "testblock.h"
#include "cpage/block.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestBlock);

using namespace cf::cpage;

void TestBlock::testInit()
{
    Block b;
    CPPUNIT_ASSERT(b.empty());
    CPPUNIT_ASSERT(b.dataPtr() == NULL);
    const Block * bptr = &b;
    CPPUNIT_ASSERT(bptr->dataPtr() == NULL);
    CPPUNIT_ASSERT(b.dataSize() == 0);
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b.userNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b.interNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b.flags());
}

void TestBlock::testSet()
{
    Block b;
    b.set((Handle) 0x1, 1, 2);
    CPPUNIT_ASSERT(b.empty());
    CPPUNIT_ASSERT_EQUAL(uint32_t(1), b.userNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b.interNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(2), b.flags());
    CPPUNIT_ASSERT_EQUAL((Handle) 0x1, b.type());

    uchar data[100];
    memset(data, 0xff, sizeof(data));
    b.set(0, 2, 3, &data, sizeof(data));
    CPPUNIT_ASSERT(!b.empty());
    CPPUNIT_ASSERT_EQUAL(uint32_t(sizeof(data)), b.dataSize());
    CPPUNIT_ASSERT(b.dataPtr());
    CPPUNIT_ASSERT(b.dataPtr() != data);
    CPPUNIT_ASSERT(b.dataPtr()[0] == 0xff);
    CPPUNIT_ASSERT(b.dataPtr()[1] == 0xff);

    data[0] = 0;

    CPPUNIT_ASSERT(b.dataPtr()[0] == 0xff);
    CPPUNIT_ASSERT(b.dataPtr()[1] == 0xff);

    uchar data2[20];
    b.setData(0, data2, sizeof(data2));

    b.setUserNum(100);
    CPPUNIT_ASSERT_EQUAL(uint32_t(100), b.userNum());
    b.setInterNum(200);
    CPPUNIT_ASSERT_EQUAL(uint32_t(200), b.interNum());
}

void TestBlock::testGet()
{
    Block b;
    b.setType(0);
    CPPUNIT_ASSERT(b.getData(0, NULL, 0) == 0);
}

void TestBlock::testCopy()
{
    char data[100];
    memset(data, 0xff, sizeof(data));
    Block b;
    b.set(0, 2, 3, &data, sizeof(data));

    // copy ctor
    Block b_copy = b;
    CPPUNIT_ASSERT(!b_copy.empty());
    CPPUNIT_ASSERT_EQUAL(b_copy.userNum(), b.userNum());
    CPPUNIT_ASSERT_EQUAL(b_copy.interNum(), b.interNum());
    CPPUNIT_ASSERT_EQUAL(b_copy.flags(), b.flags());
    CPPUNIT_ASSERT(b.dataPtr() != b_copy.dataPtr());
    CPPUNIT_ASSERT_EQUAL(b.dataSize(), b_copy.dataSize());
    CPPUNIT_ASSERT(b.dataPtr()[0] == 0xff);
    CPPUNIT_ASSERT(b.dataPtr()[1] == 0xff);
    CPPUNIT_ASSERT(b_copy.dataPtr()[0] == 0xff);
    CPPUNIT_ASSERT(b_copy.dataPtr()[1] == 0xff);

    b.dataPtr()[0] = 0;
    CPPUNIT_ASSERT(b.dataPtr()[0] == 0x0);
    CPPUNIT_ASSERT(b_copy.dataPtr()[0] == 0xff);

    // operator=
    Block b3;
    CPPUNIT_ASSERT(b3.empty());
    b3 = b;
    CPPUNIT_ASSERT(!b3.empty());
    CPPUNIT_ASSERT_EQUAL(b3.userNum(), b.userNum());
    CPPUNIT_ASSERT_EQUAL(b3.interNum(), b.interNum());
    CPPUNIT_ASSERT_EQUAL(b3.flags(), b.flags());
    CPPUNIT_ASSERT(b.dataPtr() != b3.dataPtr());
    CPPUNIT_ASSERT_EQUAL(b.dataSize(), b3.dataSize());
    CPPUNIT_ASSERT(b.dataPtr()[0] == 0x0);
    CPPUNIT_ASSERT(b.dataPtr()[1] == 0xff);
    CPPUNIT_ASSERT(b3.dataPtr()[0] == 0x0);
    CPPUNIT_ASSERT(b3.dataPtr()[1] == 0xff);

}

void TestBlock::testCompare()
{
    uchar data[10];
    memset(data, 0xff, sizeof(data));

    Block b1, b2;
    CPPUNIT_ASSERT(b1 == b2);

    b1.setData(0, data, sizeof(data));
    b2.setData(0, data, sizeof(data));

    CPPUNIT_ASSERT(b1 == b2);
    b1.setUserNum(1);
    CPPUNIT_ASSERT(b1 != b2);
    b2.setUserNum(1);
    CPPUNIT_ASSERT(b1 == b2);

    b1.dataPtr()[0] = 0;
    CPPUNIT_ASSERT(b1 != b2);

    b1.setType((Handle) 1);
    CPPUNIT_ASSERT(b1 != b2);
}

void TestBlock::testSave()
{
    Block b;
    b.setUserNum(1);
    b.setInterNum(2);
    b.setFlags(0xff);
    b.setType((Handle) 0);

    uchar data[10];
    memset(data, 0xff, sizeof(data));
    b.setData(0, data, sizeof(data));

    std::ofstream bad;
    bad << 0xDEADBEEF;
    CPPUNIT_ASSERT(!b.save(bad));

    std::ofstream null;
    CPPUNIT_ASSERT(!b.save(null));

    std::ofstream os("test_block.dat");
    CPPUNIT_ASSERT(!b.save(os));
    CPPUNIT_ASSERT(os.good());
    os.seekp(0);

    Handle t = CPAGE_GetInternalType("user type");
    b.setType(t);
    CPPUNIT_ASSERT(b.save(os));
    CPPUNIT_ASSERT(os.good());
    os.close();
}

void TestBlock::testRestore()
{
    Block b;
    int tmp;
    std::ifstream bad;
    bad >> tmp;
    CPPUNIT_ASSERT(!b.restore(bad));

    std::ifstream null;
    CPPUNIT_ASSERT(!b.restore(null));

    std::ifstream is("test_block.dat");
    CPPUNIT_ASSERT(is);
    CPPUNIT_ASSERT(b.restore(is));

    CPPUNIT_ASSERT_EQUAL(uint32_t(1), b.userNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(2), b.interNum());
    CPPUNIT_ASSERT_EQUAL(uint32_t(0xff), b.flags());

    is.close();
    CPPUNIT_ASSERT(!b.restore(is));
}

uint32_t convert(uint32_t,
                 Handle /*typeIn*/, void * /*dataIn*/, uint32_t /*sizeIn*/,
                 Handle /*typeOut*/, void * /*dataOut*/, uint32_t sizeOut) {
    return sizeOut * 2;
}

void TestBlock::testConvert()
{
    Block b;
    CPPUNIT_ASSERT(b.Convert(0, NULL, 100) == 0);

    DataConvertor c(&convert);
    Block::setConvertor(c);

    CPPUNIT_ASSERT(b.Convert(0, NULL, 100) == 100 * 2);
}
