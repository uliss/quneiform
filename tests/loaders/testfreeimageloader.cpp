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

#include "testfreeimageloader.h"
#include "rdib/freeimageloader.h"
#include "common/ctdib.h"
#include "common/tostring.h"
#include "common/cifconfig.h"
#include "common/helper.h"
#include "common/log.h"

#include "loaders_common.h"

using namespace cf;

#ifndef LOADER_TEST_IMAGE_DIR
#define LOADER_TEST_IMAGE_DIR ""
#endif

CPPUNIT_TEST_SUITE_REGISTRATION(TestFreeImageLoader);

void TestFreeImageLoader::testInit()
{
    FreeImageLoader l;
    CPPUNIT_ASSERT(!l.supportedFormats().empty());

#define CHECK_FORMAT(loader, format) CPPUNIT_ASSERT(loader.supportsFormat(format));

    CHECK_FORMAT(l, FORMAT_BMP);
    CHECK_FORMAT(l, FORMAT_PNG);
    CHECK_FORMAT(l, FORMAT_JPEG);

#undef CHECK_FORMAT

    CPPUNIT_ASSERT(!l.supportsFormat(FORMAT_PDF));
}

void TestFreeImageLoader::testLoad()
{
    const std::string IMG_PATH = LOADER_TEST_IMAGE_DIR;

    FreeImageLoader l;
    CPPUNIT_ASSERT_THROW(l.load(ImageURL()), ImageLoader::Exception);

#define ASSERT_LOAD(fname) \
    CPPUNIT_ASSERT_NO_THROW(img = l.load(ImageURL(IMG_PATH + fname)));

    ImagePtr img;
    ASSERT_LOAD("test.xpm");
    CPPUNIT_ASSERT_EQUAL(Size(1, 1), img->size());

    ImageFormatList formats = l.supportedFormats();

    for(size_t i = 0; i < formats.size(); i++) {
        if(formats[i] == cf::FORMAT_BMP)
            continue;

        std::string image_name = "test.";
        image_name += imageFormatToString(formats[i]);

        cfDebug() << METHOD_SIGNATURE() << ":" << image_name;
        ASSERT_LOAD(image_name);
    }

    ASSERT_LOAD("test.pbm");
    ASSERT_LOAD("test.pgm");
    ASSERT_LOAD("test.ppm");

    // throw
    CPPUNIT_ASSERT_THROW(l.load(ImageURL("not-exists")), ImageLoader::Exception);
    std::ifstream is_empty;
    CPPUNIT_ASSERT_THROW(l.load(is_empty), ImageLoader::Exception);
    std::ifstream is_bad;
    int tmp;
    is_bad >> tmp;
    CPPUNIT_ASSERT_THROW(l.load(is_bad), ImageLoader::Exception);

#undef ASSERT_LOAD
}

void TestFreeImageLoader::testLoadParams()
{
    FreeImageLoader l;
    const std::string IMG_PATH = LOADER_TEST_IMAGE_DIR;
    ImagePtr img;

#define ASSERT_LOAD(fname) \
    CPPUNIT_ASSERT_NO_THROW(img = l.load(ImageURL(IMG_PATH + fname)));

    ASSERT_LOAD("dpi72x72_monochrome.png");

    BitmapInfoHeader * head = (BitmapInfoHeader*) img->data();
    static const double INCH = 0.0254;
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biPlanes);
    CPPUNIT_ASSERT_EQUAL(24, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biYPelsPerMeter * INCH)));

    ASSERT_LOAD("dpi72x72_rgb.png");
    head = (BitmapInfoHeader*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biPlanes);
    CPPUNIT_ASSERT_EQUAL(24, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biYPelsPerMeter * INCH)));

    ASSERT_LOAD("dpi300x300_monochrome.png");
    head = (BitmapInfoHeader*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(24, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(300, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(300, int(round(head->biYPelsPerMeter * INCH)));

    ASSERT_LOAD("dpi_unknown.bmp");
    head = (BitmapInfoHeader*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(75, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(75, int(round(head->biYPelsPerMeter * INCH)));

#undef ASSERT_LOAD
}

void TestFreeImageLoader::testLoadRecognize() {
    FreeImageLoader loader;
    Config::instance().setDebug(false);

    // bmp
//    ASSERT_RECOGNIZE(loader, "english_32.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_24.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_16a.bmp", "ENGLISH");
//    ASSERT_RECOGNIZE(loader, "english_16b.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_gray.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_gray_rle.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_indexed.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_indexed_rle.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_1.bmp", "ENGLISH");
}

void TestFreeImageLoader::testMultiPage()
{
    FreeImageLoader loader;
    ImagePtr img;

#define LOAD_FAIL(fname, page) \
    CPPUNIT_ASSERT_THROW(img = loader.load(ImageURL(LOADER_TEST_IMAGE_DIR fname, page)), ImageLoader::Exception)
#define ASSERT_LOAD(fname, page)\
    CPPUNIT_ASSERT_NO_THROW(img = loader.load(ImageURL(LOADER_TEST_IMAGE_DIR fname, page)))

    ASSERT_LOAD("test.xpm", 0);
    LOAD_FAIL("test.xpm", 1);

    ASSERT_LOAD("multipage.tif", 0);
    CPPUNIT_ASSERT_EQUAL(img->size(), Size(281, 81));
    ASSERT_LOAD("multipage.tif", 1);
    LOAD_FAIL("multipage.tif", 2);
    CPPUNIT_ASSERT_EQUAL(img->size(), Size(81, 281));
    LOAD_FAIL("multipage.tif", -1);

#undef LOAD_FAIL
#undef ASSERT_LOAD
}
