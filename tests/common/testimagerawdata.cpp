/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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
#include "testimagerawdata.h"
#include <common/imagerawdata.h>
#include <common/tostring.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestImageRawData);

using namespace CIF;

void TestImageRawData::testInit() {
    ImageRawPtr ptr(new ImageRawData);
    CPPUNIT_ASSERT(ptr->size() == 0);
    CPPUNIT_ASSERT(ptr->data() == NULL);
    uchar * data = new unsigned char[10];
    data[0] = 1;
    ptr->set(data, 10, ImageRawData::AllocatorNew);
    CPPUNIT_ASSERT_EQUAL(data, ptr->data());
    CPPUNIT_ASSERT(ptr->size() == 10);
    CPPUNIT_ASSERT(ptr->data()[0] == 1);

    uchar data1[11];
    data1[0] = 2;
    ptr->set(data1, 11, ImageRawData::AllocatorNone);
    CPPUNIT_ASSERT(data1 == ptr->data());
    CPPUNIT_ASSERT (11 == ptr->size());
    CPPUNIT_ASSERT(2 == ptr->data()[0]);

    uchar * data2 = (uchar*) malloc(12);
    data2[0] = 3;
    ptr->set(data2, 12, ImageRawData::AllocatorMalloc);
    CPPUNIT_ASSERT_EQUAL(data2, ptr->data());
    CPPUNIT_ASSERT(ptr->size() == 12);
    CPPUNIT_ASSERT(ptr->data()[0] == 3);
}
