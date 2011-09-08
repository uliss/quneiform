/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <boost/scoped_ptr.hpp>
#include <boost/scoped_array.hpp>
#include <cstring>

#include "testsharedimage.h"
#include "puma/sharedimage.h"
#include "common/helper.h"
#include "common/tostring.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestSharedImage);

using namespace cf;

typedef boost::scoped_ptr<cf::SharedImage> PImg;

bool compare(SharedImage * shimg, ImagePtr img) {
    if(shimg->dataSize() != img->dataSize())
        return false;

    if(shimg->name() != img->fileName())
        return false;

    if(shimg->dimensions() != img->size())
        return false;

    if(memcmp(img->data(), shimg->data(), shimg->dataSize()) != 0)
        return false;

    return true;
}

void TestSharedImage::testConstruct()
{
    {
        PImg shimg(new SharedImage);
        CPPUNIT_ASSERT_EQUAL(std::string(), shimg->name());
        CPPUNIT_ASSERT_EQUAL(size_t(0), shimg->dataSize());

        ImagePtr img = shimg->get();
        CPPUNIT_ASSERT(!img.get());
    }

    {
        uchar data[100];
        ImagePtr img(new Image(data, sizeof(data), Image::AllocatorNone));
        img->setSize(Size(100, 200));
        img->setFileName("test");
        PImg shimg(new SharedImage(img));

        CPPUNIT_ASSERT(compare(shimg.get(), img));

        ImagePtr res = shimg->get();
        CPPUNIT_ASSERT(compare(shimg.get(), res));
    }
}

void TestSharedImage::testSet()
{
    // empty
    {
        ImagePtr img;
        PImg shimg(new SharedImage());
        shimg->set(img);
        CPPUNIT_ASSERT_EQUAL(std::string(), shimg->name());
        CPPUNIT_ASSERT_EQUAL(size_t(0), shimg->dataSize());
    }

    // too big
    {
        boost::scoped_array<uchar> data(new uchar[sizeof(SharedImage)]);
        ImagePtr img(new Image(data.get(), sizeof(SharedImage), Image::AllocatorNone));
        PImg shimg(new SharedImage);
        shimg->set(img);

        CPPUNIT_ASSERT(shimg->dataSize() == 0);
        CPPUNIT_ASSERT(shimg->name().empty());
        CPPUNIT_ASSERT(shimg->dimensions().height() == 0);
        CPPUNIT_ASSERT(shimg->dimensions().width() == 0);
    }

    // normal
    {
        uchar data[100];
        data[0] = 0;
        data[1] = 1;
        data[2] = 2;
        data[3]= 0;

        ImagePtr img(new Image(data, sizeof(data), Image::AllocatorNone));
        img->setSize(Size(200, 300));
        img->setFileName("test");

        PImg shimg(new SharedImage);
        shimg->set(img);

        CPPUNIT_ASSERT(compare(shimg.get(), img));
    }

    // file name too long
    {
        uchar data[100];
        data[0] = 0;
        data[1] = 1;
        data[2] = 2;
        data[3]= 0;

        ImagePtr img(new Image(data, sizeof(data), Image::AllocatorNone));
        img->setSize(Size(200, 300));
        img->setFileName(std::string(1024, 'c'));

        PImg shimg(new SharedImage);
        shimg->set(img);

        CPPUNIT_ASSERT(!compare(shimg.get(), img));
    }
}

void TestSharedImage::testClear() {
    uchar data[100];
    data[0] = 0;
    data[1] = 1;
    data[2] = 2;
    data[3]= 0;

    ImagePtr img(new Image(data, sizeof(data), Image::AllocatorNone));
    img->setSize(Size(200, 300));
    img->setFileName("test");

    PImg shimg(new SharedImage(img));

    CPPUNIT_ASSERT(compare(shimg.get(), img));
    ImagePtr res = shimg->get();
    CPPUNIT_ASSERT(compare(shimg.get(), res));

    shimg->clear();
    CPPUNIT_ASSERT(!compare(shimg.get(), res));
}
