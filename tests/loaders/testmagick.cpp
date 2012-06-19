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
#include "testmagick.h"
#include <memory>
#include <fstream>
#include <math.h>

#include "loaders_common.h"
#include "rdib/magickimageloader.h"
#include "rdib/ctdib.h"
#include "common/cifconfig.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestMagickLoader);
using namespace cf;

typedef std::auto_ptr<MagickImageLoader> LoaderPtr;

#ifndef LOADER_TEST_IMAGE_DIR
#define LAODER_TEST_IMAGE_DIR ""
#endif

void TestMagickLoader::testInit() {
    LoaderPtr loader(new MagickImageLoader);
}

void TestMagickLoader::testLoad() {
    std::string path = LOADER_TEST_IMAGE_DIR;
    LoaderPtr loader(new MagickImageLoader);
    ImagePtr img;
    CPPUNIT_ASSERT_NO_THROW(img = loader->load(path + "test.xpm"));
    CPPUNIT_ASSERT(img->size() == Size(1, 1));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.bmp"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.gif"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.tif"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.jpg"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.png"));
#if !defined(__OpenBSD__)
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.pbm"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.pgm"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.ppm"));
#endif
    // throw
    CPPUNIT_ASSERT_THROW(loader->load("not-exists"), ImageLoader::Exception);
    std::ifstream is_empty;
    CPPUNIT_ASSERT_THROW(loader->load(is_empty), ImageLoader::Exception);
    std::ifstream is_bad;
    int tmp;
    is_bad >> tmp;
    CPPUNIT_ASSERT_THROW(loader->load(is_bad), ImageLoader::Exception);

}

void TestMagickLoader::testLoadParams()
{
    std::string path = LOADER_TEST_IMAGE_DIR;
    MagickImageLoader loader;
    ImagePtr img = loader.load(path + "dpi72x72_monochrome.png");

    BitmapInfoHeader * head = (BitmapInfoHeader*) img->data();
    static const double INCH = 0.0254;
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biPlanes);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biYPelsPerMeter * INCH)));

    img = loader.load(path + "dpi72x72_rgb.png");
    head = (BitmapInfoHeader*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biPlanes);
    CPPUNIT_ASSERT_EQUAL(24, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biYPelsPerMeter * INCH)));

    img = loader.load(path + "dpi300x300_monochrome.png");
    head = (BitmapInfoHeader*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(300, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(300, int(round(head->biYPelsPerMeter * INCH)));

    img = loader.load(path + "dpi_unknown.bmp");
    head = (BitmapInfoHeader*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(75, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(75, int(round(head->biYPelsPerMeter * INCH)));
}

void TestMagickLoader::testLoadRecognize() {
    MagickImageLoader loader;
    Config::instance().setDebug(false);

    // bmp
    ASSERT_RECOGNIZE(loader, "english_32.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_24.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_16a.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_16b.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_gray.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_gray_rle.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_indexed.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_indexed_rle.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_1.bmp", "ENGLISH");
}
