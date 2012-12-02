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

#include <fstream>
#include <string.h>

#include "testimage.h"
#include "../test_common.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestImage);
#include "common/image.h"
#include "common/tostring.h"
using namespace cf;

void TestImage::testInit() {
    ImagePtr im(new Image(NULL, 0, Image::AllocatorMalloc));

    uchar * data1 = new uchar[100];
    Image im1(data1, 100, Image::AllocatorNew);
    uchar * data2 = (uchar*) malloc(50);
    Image im2(data2, 50, Image::AllocatorMalloc);

    CPPUNIT_ASSERT(im1.fileName().empty());
    CPPUNIT_ASSERT_EQUAL(data1, im1.data());
}

void TestImage::testSerialize() {
#ifdef CF_SERIALIZE
    const char * FNAME = "serialize_image.txt";
    uchar * data = new uchar[100];
    Image im(data, 100, Image::AllocatorNew);
    im.setFileName("test image.png");
    im.setSize(Size(20, 40));

    writeToTextArchive(FNAME, im);

    Image new_img(NULL, 0, Image::AllocatorNew);
    readFromTextArchive(FNAME, new_img);

    CPPUNIT_ASSERT_EQUAL(im.fileName(), new_img.fileName());
    CPPUNIT_ASSERT_EQUAL(im.size(), new_img.size());
    CPPUNIT_ASSERT_EQUAL(im.width(), new_img.width());
    CPPUNIT_ASSERT_EQUAL(im.height(), new_img.height());

#endif
}

void TestImage::testSerializeXml() {
#ifdef CF_SERIALIZE
    const char * FNAME = "serialize_image.xml";
    uchar * data = new uchar[100];
    Image im(data, 100, Image::AllocatorNew);
    im.setFileName("test <>&image.png");
    im.setSize(Size(20, 40));

    writeToXmlArchive(FNAME, "image", im);

    Image new_img(NULL, 0, Image::AllocatorNew);
    readFromXmlArchive(FNAME, "image", new_img);

    CPPUNIT_ASSERT_EQUAL(im.fileName(), new_img.fileName());
    CPPUNIT_ASSERT_EQUAL(im.size(), new_img.size());
    CPPUNIT_ASSERT_EQUAL(im.width(), new_img.width());
    CPPUNIT_ASSERT_EQUAL(im.height(), new_img.height());

#endif
}

void TestImage::testClone()
{
    Image im;
    im.setSize(Size(10, 20));
    im.setFileName("test");

    Image * im_copy = im.clone();
    CPPUNIT_ASSERT(im_copy);
    CPPUNIT_ASSERT_EQUAL(im.size(), im_copy->size());
    CPPUNIT_ASSERT_EQUAL(im.fileName(), im_copy->fileName());
    CPPUNIT_ASSERT(im_copy->isNull());
    delete im_copy;

    uchar data[100];
    memset(data, 0xFF, 100);
    im.set(data, 100, Image::AllocatorNone);

    im_copy = im.clone();
    CPPUNIT_ASSERT(im_copy);
    CPPUNIT_ASSERT_EQUAL(im.size(), im_copy->size());
    CPPUNIT_ASSERT_EQUAL(im.fileName(), im_copy->fileName());
    CPPUNIT_ASSERT(!im_copy->isNull());
    CPPUNIT_ASSERT(im.data() != im_copy->data());
    CPPUNIT_ASSERT_EQUAL(im.dataSize(), im_copy->dataSize());
    delete im_copy;

}
