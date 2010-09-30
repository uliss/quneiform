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
#include <cstring>
#include <fstream>
#include "../test_common.h"
#include <common/tostring.h>
#define private public
#include <common/imagerawdata.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestImageRawData);

using namespace cf;

void TestImageRawData::testInit() {
    ImageRawPtr ptr(new ImageRawData);
    CPPUNIT_ASSERT(ptr->dataSize() == 0);
    CPPUNIT_ASSERT(ptr->data() == NULL);
    uchar * data = new unsigned char[10];
    data[0] = 1;
    ptr->set(data, 10, ImageRawData::AllocatorNew);
    CPPUNIT_ASSERT_EQUAL(data, ptr->data());
    CPPUNIT_ASSERT(ptr->dataSize() == 10);
    CPPUNIT_ASSERT(ptr->data()[0] == 1);

    uchar data1[11];
    data1[0] = 2;
    ptr->set(data1, 11, ImageRawData::AllocatorNone);
    CPPUNIT_ASSERT(data1 == ptr->data());
    CPPUNIT_ASSERT (11 == ptr->dataSize());
    CPPUNIT_ASSERT(2 == ptr->data()[0]);

    uchar * data2 = (uchar*) malloc(12);
    data2[0] = 3;
    ptr->set(data2, 12, ImageRawData::AllocatorMalloc);
    CPPUNIT_ASSERT_EQUAL(data2, ptr->data());
    CPPUNIT_ASSERT(ptr->dataSize() == 12);
    CPPUNIT_ASSERT(ptr->data()[0] == 3);
}

void TestImageRawData::testSerialize() {
#ifdef CF_SERIALIZE
    ImageRawData img;
    img.set(new uchar[10], 10, ImageRawData::AllocatorNew);
    memset(img.data(), 0x21, 10);

    const char * FNAME = "serialize_imageraw.txt";
    writeToTextArchive(FNAME, img);

    ImageRawData new_img;
    readFromTextArchive(FNAME, new_img);

    CPPUNIT_ASSERT_EQUAL(ImageRawData::AllocatorNew, new_img.allocator_);
    CPPUNIT_ASSERT_EQUAL(img.dataSize(), new_img.dataSize());
    for(int i = 0; i < 10; i++) {
        CPPUNIT_ASSERT(new_img.data()[i] == 0x21);
    }

    unsigned char data1[10];
    data1[0] = 1;
    img.set(data1, sizeof(data1), ImageRawData::AllocatorNone);

    writeToTextArchive(FNAME, img);

    {
        unsigned char data2[10];
        data2[0] = 2;
        ImageRawData new_img(data2, 10, ImageRawData::AllocatorNone);
        CPPUNIT_ASSERT_EQUAL(uchar(2), new_img.data()[0]);

        readFromTextArchive(FNAME, new_img);
        CPPUNIT_ASSERT(new_img.dataSize() == 10);
        CPPUNIT_ASSERT_EQUAL(ImageRawData::AllocatorNew, new_img.allocator_);
        CPPUNIT_ASSERT_EQUAL(uchar(1), new_img.data()[0]);
    }
#endif
}

void TestImageRawData::testSerializeXml() {
#ifdef CF_SERIALIZE
    ImageRawData img;
    img.set(new uchar[10], 10, ImageRawData::AllocatorNew);
    memset(img.data(), 0x21, 10);

    const char * FNAME = "serialize_imageraw.xml";

    writeToXmlArchive(FNAME, "image-raw", img);

    ImageRawData new_img;
    readFromXmlArchive(FNAME, "image-raw", new_img);

    CPPUNIT_ASSERT_EQUAL(ImageRawData::AllocatorNew, new_img.allocator_);
    CPPUNIT_ASSERT_EQUAL(img.dataSize(), new_img.dataSize());
    for(int i = 0; i < 10; i++) {
        CPPUNIT_ASSERT(new_img.data()[i] == 0x21);
    }
#endif
}
