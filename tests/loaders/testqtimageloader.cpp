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

#include <memory>
#include <sstream>
#include <fstream>
#include <math.h>

#include "testqtimageloader.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestQtImageLoader);

#include "loaders_common.h"
#include "common/debug.h"
#include "common/cifconfig.h"
#include "rdib/qtimageloader.h"
#include "rdib/ctdib.h"

using namespace cf;

#ifndef LOADER_TEST_IMAGE_DIR
#define LOADER_TEST_IMAGE_DIR ""
#endif

void TestQtImageLoader::testInit() {
    std::auto_ptr<QtImageLoader> loader(new QtImageLoader);
}

void TestQtImageLoader::testLoad() {
    std::string path = LOADER_TEST_IMAGE_DIR;
    std::auto_ptr<QtImageLoader> loader(new QtImageLoader);
    ImagePtr img;
    CPPUNIT_ASSERT_NO_THROW(img = loader->load(path + "test.xpm"));
    CPPUNIT_ASSERT(Size(1, 1) == img->size());


    ImageFormatList formats = loader->supportedFormats();

    for(size_t i = 0; i < formats.size(); i++) {
        if(formats[i] == cf::FORMAT_BMP)
            continue;

        std::string image_name = path + "test.";
        image_name += imageFormatToString(formats[i]);

        std::cerr << "CHECKING " << image_name << "\n";
        CPPUNIT_ASSERT_NO_THROW(loader->load(image_name));
    }

    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.pbm"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.pgm"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.ppm"));

    // throw
    CPPUNIT_ASSERT_THROW(loader->load("not-exists"), ImageLoader::Exception);
    std::ifstream is_empty;
    CPPUNIT_ASSERT_THROW(loader->load(is_empty), ImageLoader::Exception);
    std::ifstream is_bad;
    int tmp;
    is_bad >> tmp;
    CPPUNIT_ASSERT_THROW(loader->load(is_bad), ImageLoader::Exception);
}

void TestQtImageLoader::testLoadRecognize() {
    QtImageLoader loader;
    Config::instance().setDebug(false);

    // bmp
//    ASSERT_RECOGNIZE("english_32.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_24.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_16a.bmp", "ENGLISH");
//    ASSERT_RECOGNIZE("english_16b.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_gray.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_gray_rle.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_indexed.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_indexed_rle.bmp", "ENGLISH");
    ASSERT_RECOGNIZE(loader, "english_1.bmp", "ENGLISH");
}

void TestQtImageLoader::testLoadParams()
{
    QtImageLoader loader;
    std::string path = LOADER_TEST_IMAGE_DIR;
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
