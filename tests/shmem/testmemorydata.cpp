/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavski                                 *
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

#include "testmemorydata.h"
#include "shmem/memorydata.h"
#include "ced/cedpage.h"
#include "common/tostring.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestMemoryData);

using namespace cf;

void TestMemoryData::testInit()
{
    MemoryData data(NULL, 0);
    CPPUNIT_ASSERT(data.empty());
    CPPUNIT_ASSERT(data.memory() == NULL);
    CPPUNIT_ASSERT(data.size() == 0);
    CPPUNIT_ASSERT_THROW(data.formatOptions(), MemoryData::Exception);
}

void TestMemoryData::testFormatOptions()
{
    {
        // test NULL
        MemoryData data(NULL, 0);
        CPPUNIT_ASSERT_THROW(data.formatOptions(), MemoryData::Exception);
        CPPUNIT_ASSERT_THROW(data.setFormatOptions(FormatOptions()), MemoryData::Exception);
    }

    {
        // test small size
        char ch;
        MemoryData data(&ch, sizeof(ch));
        CPPUNIT_ASSERT_THROW(data.formatOptions(), MemoryData::Exception);
        CPPUNIT_ASSERT_THROW(data.setFormatOptions(FormatOptions()), MemoryData::Exception);
    }

    {
        // test null format options
        const size_t SZ = MemoryData::minBufferSize();
        char * mem = new char[SZ];
        MemoryData data(mem, SZ);
        data.clear();
        CPPUNIT_ASSERT_NO_THROW(data.formatOptions());
        // garbage data
        memset(mem, 0xFF, SZ);
        CPPUNIT_ASSERT_THROW(data.formatOptions(), MemoryData::Exception);
        delete[] mem;
    }

    {
        const size_t BUF_SIZE = MemoryData::minBufferSize();
        char * buf = new char[BUF_SIZE];

        {
            MemoryData data(buf, BUF_SIZE);
            data.clear();
            FormatOptions fopts;
            fopts.setSansSerifName("Unknown123");
            data.setFormatOptions(fopts);
            data.formatOptions();
        }

        {
            MemoryData data(buf, BUF_SIZE);
            FormatOptions fopts = data.formatOptions();
            CPPUNIT_ASSERT_EQUAL(std::string("Unknown123"), fopts.sansSerifName());
        }

        delete[] buf;
    }
}

void TestMemoryData::testRecognizeOptions()
{
    {
        // test NULL
        MemoryData data(NULL, 0);
        CPPUNIT_ASSERT_THROW(data.recognizeOptions(), MemoryData::Exception);
        CPPUNIT_ASSERT_THROW(data.setRecognizeOptions(RecognizeOptions()), MemoryData::Exception);
    }

    {
        // test small size
        char ch;
        MemoryData data(&ch, sizeof(ch));
        CPPUNIT_ASSERT_THROW(data.recognizeOptions(), MemoryData::Exception);
        CPPUNIT_ASSERT_THROW(data.setRecognizeOptions(RecognizeOptions()), MemoryData::Exception);
    }

    {
        // test null recognize options
        const size_t SZ = MemoryData::minBufferSize();
        char * mem = new char[SZ];
        MemoryData data(mem, SZ);
        data.clear();
        CPPUNIT_ASSERT_NO_THROW(data.recognizeOptions());
        // garbage data
        memset(mem, 0xFF, SZ);
        CPPUNIT_ASSERT_THROW(data.recognizeOptions(), MemoryData::Exception);
        delete[] mem;
    }

    {
        const size_t BUF_SIZE = MemoryData::minBufferSize();
        char * buf = new char[BUF_SIZE];

        {
            MemoryData data(buf, BUF_SIZE);
            data.clear();
            RecognizeOptions ropts;
            ropts.setUserDict("Fake dict");
            data.setRecognizeOptions(ropts);
            data.recognizeOptions();
        }

        {
            MemoryData data(buf, BUF_SIZE);
            RecognizeOptions ropts = data.recognizeOptions();
            CPPUNIT_ASSERT_EQUAL(std::string("Fake dict"), ropts.userDict());
        }

        delete[] buf;
    }
}

void TestMemoryData::testPage()
{
    {
        // test NULL
        MemoryData data(NULL, 0);
        CPPUNIT_ASSERT_THROW(data.page(), MemoryData::Exception);
        CPPUNIT_ASSERT_THROW(data.setPage(CEDPagePtr()), MemoryData::Exception);
    }

    {
        // test small size
        char ch;
        MemoryData data(&ch, sizeof(ch));
        CPPUNIT_ASSERT_THROW(data.page(), MemoryData::Exception);
        CPPUNIT_ASSERT_THROW(data.setPage(CEDPagePtr()), MemoryData::Exception);
    }

    {
        // test null page data
        const size_t SZ = MemoryData::minBufferSize();
        char * mem = new char[SZ];
        MemoryData data(mem, SZ);
        data.clear();
        CPPUNIT_ASSERT_NO_THROW(data.page());

        // garbage data
        memset(mem, 0xFF, SZ);
        CPPUNIT_ASSERT_THROW(data.page(), MemoryData::Exception);
        delete[] mem;
    }

    {
        const size_t BUF_SIZE = MemoryData::minBufferSize();
        char * buf = new char[BUF_SIZE];

        {
            MemoryData data(buf, BUF_SIZE);
            data.clear();
            CEDPagePtr page(new CEDPage);
            page->setImageName("Test page");
            data.setPage(page);
            CPPUNIT_ASSERT(data.page().get());
        }

        {
            MemoryData data(buf, BUF_SIZE);
            CEDPagePtr page = data.page();
            CPPUNIT_ASSERT_EQUAL(std::string("Test page"), page->imageName());
        }

        delete[] buf;
    }
}

void TestMemoryData::testImagePath()
{
    {
        // test NULL
        MemoryData data(NULL, 0);
        CPPUNIT_ASSERT_THROW(data.imageURL(), MemoryData::Exception);
        CPPUNIT_ASSERT_THROW(data.setImageURL(ImageURL("some path")), MemoryData::Exception);
    }

    {
        // test small size
        char ch;
        MemoryData data(&ch, sizeof(ch));
        CPPUNIT_ASSERT_THROW(data.imageURL(), MemoryData::Exception);
        CPPUNIT_ASSERT_THROW(data.setImageURL(ImageURL("some path")), MemoryData::Exception);
    }

    {
        // test null page data
        const size_t SZ = MemoryData::minBufferSize();
        char * mem = new char[SZ];
        MemoryData data(mem, SZ);
        data.clear();
        CPPUNIT_ASSERT_NO_THROW(data.imageURL());

        // garbage data
        memset(mem, 0xFF, SZ);
        CPPUNIT_ASSERT_THROW(data.imageURL(), MemoryData::Exception);
        delete[] mem;
    }

    {
        const size_t BUF_SIZE = MemoryData::minBufferSize();
        char * buf = new char[BUF_SIZE];

        {
            MemoryData data(buf, BUF_SIZE);
            data.clear();
            data.setImageURL(ImageURL("test path", 2));
            data.imageURL();
        }

        {
            MemoryData data(buf, BUF_SIZE);
            CPPUNIT_ASSERT_EQUAL(ImageURL("test path", 2), data.imageURL());
            // too long path
            CPPUNIT_ASSERT_THROW(data.setImageURL(ImageURL(std::string(2000, '1'))), MemoryData::Exception);
        }

        delete[] buf;
    }
}

void TestMemoryData::testImage()
{
    {
        MemoryData data(NULL, 0);
        // NULL memory
        CPPUNIT_ASSERT_THROW(data.image(), MemoryData::Exception);
        // NULL image
        CPPUNIT_ASSERT_THROW(data.setImage(ImagePtr()), MemoryData::Exception);

        // NULL size
        uchar image_data[100];
        ImagePtr ip(new Image(image_data, 100, Image::AllocatorNone));
        CPPUNIT_ASSERT_THROW(data.setImage(ip), MemoryData::Exception);
    }

    {
        // test small size
        char ch;
        MemoryData data(&ch, sizeof(ch));
        CPPUNIT_ASSERT_THROW(data.image(), MemoryData::Exception);

        char * mem = new char[MemoryData::minBufferSize()];

        uchar image_data[100];
        ImagePtr ip(new Image(image_data, 100, Image::AllocatorNone));
        CPPUNIT_ASSERT_THROW(data.setImage(ip), MemoryData::Exception);

        delete[] mem;
    }

    {
        // test null image data
        const size_t SZ = MemoryData::minBufferSize();
        char * mem = new char[SZ];
        MemoryData data(mem, SZ);
        data.clear();
        CPPUNIT_ASSERT_NO_THROW(data.image());

        // garbage data
        memset(mem, 0xFF, SZ);
        CPPUNIT_ASSERT_THROW(data.image(), MemoryData::Exception);
        delete[] mem;
    }

    {
        const size_t SZ = MemoryData::minBufferSize() + 100;
        char * buf = new char[SZ];
        uchar image_data[100];
        image_data[1] = 10;

        {
            MemoryData data(buf, SZ);
            data.clear();

            ImagePtr ip(new Image(image_data, 100, Image::AllocatorNone));
            ip->setSize(Size(10, 15));
            ip->setFileName("test image");
            data.setImage(ip);
            CPPUNIT_ASSERT(data.image().get());
            CPPUNIT_ASSERT(data.image()->data()[1] == 10);
        }

        {
            MemoryData data(buf, SZ);
            ImagePtr ip = data.image();
            CPPUNIT_ASSERT(memcmp(image_data, ip->data(), ip->dataSize()) == 0);
            CPPUNIT_ASSERT(data.image()->data()[1] == 10);
            CPPUNIT_ASSERT_EQUAL(Size(10, 15), ip->size());
            CPPUNIT_ASSERT_EQUAL(std::string("test image"), ip->fileName());

            // test image filename too long
            ip->setFileName(std::string(256, '1'));
            data.setImage(ip);
            CPPUNIT_ASSERT_EQUAL(std::string(256, '1'), data.image()->fileName());

            ip->setFileName(std::string(257, '1'));
            data.setImage(ip);
            CPPUNIT_ASSERT(std::string(257, '1') != data.image()->fileName());
        }

        delete[] buf;
    }
}
