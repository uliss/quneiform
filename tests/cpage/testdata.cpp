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
    TData s;
    Handle t = CPAGE_GetInternalType("new type");
    s.setType(t);

    {
        std::ofstream os("test_cpage_data.dat");
        CPPUNIT_ASSERT(os);
        CPPUNIT_ASSERT(s.save(os));
        os.close();
    }

    TData d;
    std::ifstream null;
    CPPUNIT_ASSERT(!d.restore(null));
    std::ifstream is("test_cpage_data.dat");
    CPPUNIT_ASSERT(is);
    CPPUNIT_ASSERT(d.restore(is));


    is.close();
}
