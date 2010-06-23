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
#include "testimage.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestImage);
#include <common/image.h>
#include <common/tostring.h>
#include <ced/cedarchive.h>
using namespace CIF;

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
    uchar * data = new uchar[100];
    Image im(data, 100, Image::AllocatorNew);
    im.setFileName("test image.png");
    im.setSize(Size(20, 40));

    // save data to archive
    {
        std::ofstream ofs("serialize_image.txt");
        CEDOutputArchive oa(ofs);
        // write class instance to archive
        oa << im;
    }

    Image new_img(NULL, 0, Image::AllocatorNew);
    {
        // create and open an archive for input
        std::ifstream ifs("serialize_image.txt");
        CEDInputArchive ia(ifs);
        // read class state from archive
        ia >> new_img;

        CPPUNIT_ASSERT_EQUAL(im.fileName(), new_img.fileName());
        CPPUNIT_ASSERT_EQUAL(im.size(), new_img.size());
        CPPUNIT_ASSERT_EQUAL(im.width(), new_img.width());
        CPPUNIT_ASSERT_EQUAL(im.height(), new_img.height());
    }
#endif
}
