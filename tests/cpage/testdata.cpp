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

#include <fstream>

#include "testdata.h"
#include "cpage/data.h"
#include "cpage/cpage.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestData);

class TData : public cf::cpage::Data {
public:
    uint32_t Convert(Handle /*type*/, void * /*lpdata*/, uint32_t /*size*/) {
        return 11;
    }
};

void TestData::testInit()
{
    TData d;
    d.setType(0);
    CPPUNIT_ASSERT(d.empty());
    CPPUNIT_ASSERT(d.dataSize() == 0);
    CPPUNIT_ASSERT(d.dataPtr() == NULL);
}

void TestData::testGetData()
{
    TData d;
    d.setType(0);
    CPPUNIT_ASSERT(!d.getDataPtr(0, NULL));
    void * data = 0;
    CPPUNIT_ASSERT(d.getDataPtr(0, &data));
    CPPUNIT_ASSERT(!d.getDataPtr((Handle)0x1, &data));

    CPPUNIT_ASSERT_EQUAL(uint32_t(11), d.getData((Handle)(0x2), NULL, 0));
}

void TestData::testSave()
{
    TData d;
    std::ofstream bad;
    bad << "bad";
    CPPUNIT_ASSERT(!d.save(bad));

    Handle type = CPAGE_GetInternalType("usertype");
    d.setType(type);

    std::ofstream null;
    CPPUNIT_ASSERT(!d.save(null));
}

void TestData::testRestore()
{
    {
        TData data_source_1;
        Handle t = CPAGE_GetInternalType("new type");
        data_source_1.setType(t);
        std::ofstream out_file_1("test_cpage_data.dat");
        CPPUNIT_ASSERT(out_file_1);
        CPPUNIT_ASSERT(data_source_1.save(out_file_1));
        out_file_1.close();
    }

    TData destination_data;
    std::ifstream null;
    CPPUNIT_ASSERT(!destination_data.restore(null));
    std::ifstream in_file_1("test_cpage_data.dat");
    CPPUNIT_ASSERT(in_file_1);
    CPPUNIT_ASSERT(destination_data.restore(in_file_1));
    in_file_1.close();

    CPPUNIT_ASSERT(destination_data.dataSize() == 0);
    uchar buffer_1[10];
    memset(buffer_1, 0xff, sizeof(buffer_1));
    destination_data.setData(0, buffer_1, sizeof(buffer_1));
    CPPUNIT_ASSERT(destination_data.dataSize() == 10);

    {
        TData data_source_2;
        Handle t = CPAGE_GetInternalType("new type2");
        uchar buffer_2[5];
        memset(buffer_2, 0xff, sizeof(buffer_2));
        data_source_2.setData(t, buffer_2, sizeof(buffer_2));
        CPPUNIT_ASSERT(data_source_2.dataPtr());
        CPPUNIT_ASSERT(data_source_2.dataSize() == 5);
        std::ofstream out_file_2("test_cpage_data2.dat");
        CPPUNIT_ASSERT(out_file_2);
        CPPUNIT_ASSERT(data_source_2.save(out_file_2));
        out_file_2.close();
    }

    std::ifstream in_file_2("test_cpage_data2.dat");
    CPPUNIT_ASSERT(in_file_2);
    CPPUNIT_ASSERT(destination_data.restore(in_file_2));
    CPPUNIT_ASSERT(destination_data.dataPtr());
    CPPUNIT_ASSERT(destination_data.dataSize() == 5);

    // double restore
    in_file_2.seekg(0);
    CPPUNIT_ASSERT(destination_data.restore(in_file_2));
    // double restore
    in_file_2.seekg(0);
    CPPUNIT_ASSERT(destination_data.restore(in_file_2));
    in_file_2.close();
}
