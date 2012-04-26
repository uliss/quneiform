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
#include "rdib/ctdib.h"
#include "common/tostring.h"
#include "common/cifconfig.h"

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
}

void TestFreeImageLoader::testLoad()
{
    const std::string IMG_PATH = LOADER_TEST_IMAGE_DIR;

    FreeImageLoader l;
    CPPUNIT_ASSERT_THROW(l.load(""), ImageLoader::Exception);

    ImagePtr img;
    CPPUNIT_ASSERT_NO_THROW(img = l.load(IMG_PATH + "test.xpm"));
    CPPUNIT_ASSERT_EQUAL(Size(1, 1), img->size());

    ImageFormatList formats = l.supportedFormats();

    for(size_t i = 0; i < formats.size(); i++) {
        if(formats[i] == cf::FORMAT_BMP)
            continue;

        std::string image_name = IMG_PATH + "test.";
        image_name += imageFormatToString(formats[i]);

        std::cerr << "CHECKING " << image_name << "\n";
        CPPUNIT_ASSERT_NO_THROW(l.load(image_name));
    }

    CPPUNIT_ASSERT_NO_THROW(l.load(IMG_PATH + "test.pbm"));
    CPPUNIT_ASSERT_NO_THROW(l.load(IMG_PATH + "test.pgm"));
    CPPUNIT_ASSERT_NO_THROW(l.load(IMG_PATH + "test.ppm"));

    // throw
    CPPUNIT_ASSERT_THROW(l.load("not-exists"), ImageLoader::Exception);
    std::ifstream is_empty;
    CPPUNIT_ASSERT_THROW(l.load(is_empty), ImageLoader::Exception);
    std::ifstream is_bad;
    int tmp;
    is_bad >> tmp;
    CPPUNIT_ASSERT_THROW(l.load(is_bad), ImageLoader::Exception);

}

void TestFreeImageLoader::testLoadParams()
{
    FreeImageLoader loader;
    const std::string IMG_PATH = LOADER_TEST_IMAGE_DIR;
    ImagePtr img = loader.load(IMG_PATH + "dpi72x72_monochrome.png");

    CTDIBBITMAPINFOHEADER * head = (CTDIBBITMAPINFOHEADER*) img->data();
    static const double INCH = 0.0254;
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biPlanes);
    CPPUNIT_ASSERT_EQUAL(24, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biYPelsPerMeter * INCH)));

    img = loader.load(IMG_PATH + "dpi72x72_rgb.png");
    head = (CTDIBBITMAPINFOHEADER*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biPlanes);
    CPPUNIT_ASSERT_EQUAL(24, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biYPelsPerMeter * INCH)));

    img = loader.load(IMG_PATH + "dpi300x300_monochrome.png");
    head = (CTDIBBITMAPINFOHEADER*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(24, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(300, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(300, int(round(head->biYPelsPerMeter * INCH)));

    img = loader.load(IMG_PATH + "dpi_unknown.bmp");
    head = (CTDIBBITMAPINFOHEADER*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(75, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(75, int(round(head->biYPelsPerMeter * INCH)));
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
