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
#include <QImage>

#include "testqtimageloader.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestQtImageLoader);

#include "loaders_common.h"
#include "common/log.h"
#include "common/cifconfig.h"
#include "rdib/qtimageloader.h"
#include "common/ctdib.h"

using namespace cf;

#ifndef LOADER_TEST_IMAGE_DIR
#define LOADER_TEST_IMAGE_DIR ""
#endif

#define ASSERT_LOAD(loader, name) \
    cfDebug() << "LOADING " LOADER_TEST_IMAGE_DIR name "\n";\
    CPPUNIT_ASSERT_NO_THROW(loader.load(ImageURL(LOADER_TEST_IMAGE_DIR name)))

#define ASSERT_LOAD_PAGE(loader, name, page) \
    cfDebug() << "LOADING "LOADER_TEST_IMAGE_DIR name "; page: " << page << "\n";\
    CPPUNIT_ASSERT_NO_THROW(loader.load(ImageURL(LOADER_TEST_IMAGE_DIR name, page)))

#define FORMAT_SUPPORTED(loader, fmt) \
    CPPUNIT_ASSERT(loader.supportsFormat(fmt))

#define ASSERT_LOAD_SIZE(loader, fname, sz)\
    CPPUNIT_ASSERT_NO_THROW(loader.load(ImageURL(LOADER_TEST_IMAGE_DIR fname))->size() == sz)

void TestQtImageLoader::testInit()
{
    QtImageLoader loader;
    CPPUNIT_ASSERT(!loader.supportedFormats().empty());
}

void TestQtImageLoader::testLoad()
{
    QtImageLoader loader;
    ImagePtr img;

    ASSERT_LOAD_SIZE(loader, "test.xpm", Size(1, 1));

#if (QT_VERSION >= QT_VERSION_CHECK(4, 7, 2))
    // found that qt 4.6.3 can't open this images
    // probably bug of this test image or qt... so disable this test
    ASSERT_LOAD(loader, "test.bmp");
#endif

    ASSERT_LOAD(loader, "test.jpg");
    ASSERT_LOAD(loader, "test.png");
    ASSERT_LOAD(loader, "test.pnm");
    ASSERT_LOAD(loader, "test.xpm");
    ASSERT_LOAD(loader, "test.pbm");
    ASSERT_LOAD(loader, "test.pgm");
    ASSERT_LOAD(loader, "test.ppm");

 
    if(loader.supportsFormat(FORMAT_TIFF)){
        ASSERT_LOAD(loader, "test.tif");
        // multi page tiff load
        ASSERT_LOAD_PAGE(loader, "multipage.tif", 0);
        ASSERT_LOAD_PAGE(loader, "multipage.tif", 1);
    }

    if(loader.supportsFormat(FORMAT_PDF)) {
        // multi page pdf load
        ASSERT_LOAD_PAGE(loader, "test.pdf", 0);
        ASSERT_LOAD_PAGE(loader, "test.pdf", 1);
    }

    // throw
    CPPUNIT_ASSERT_THROW(loader.load(ImageURL("not-exists")), ImageLoader::Exception);
    std::ifstream is_empty;
    CPPUNIT_ASSERT_THROW(loader.load(is_empty), ImageLoader::Exception);
    std::ifstream is_bad;
    int tmp;
    is_bad >> tmp;
    CPPUNIT_ASSERT_THROW(loader.load(is_bad), ImageLoader::Exception);
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
    ImagePtr img = loader.load(ImageURL(path + "dpi72x72_monochrome.png"));

    BitmapInfoHeader * head = (BitmapInfoHeader*) img->data();
    static const double INCH = 0.0254;
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biPlanes);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biYPelsPerMeter * INCH)));

    img = loader.load(ImageURL(path + "dpi72x72_rgb.png"));
    head = (BitmapInfoHeader*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biPlanes);
    CPPUNIT_ASSERT_EQUAL(24, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(72, int(round(head->biYPelsPerMeter * INCH)));

    img = loader.load(ImageURL(path + "dpi300x300_monochrome.png"));
    head = (BitmapInfoHeader*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(1, (int) head->biBitCount);
    CPPUNIT_ASSERT_EQUAL(300, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(300, int(round(head->biYPelsPerMeter * INCH)));

    img = loader.load(ImageURL(ImageURL(path + "dpi_unknown.bmp")));
    head = (BitmapInfoHeader*) img->data();
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biWidth);
    CPPUNIT_ASSERT_EQUAL(10, (int) head->biHeight);
    CPPUNIT_ASSERT_EQUAL(75, int(round(head->biXPelsPerMeter * INCH)));
    CPPUNIT_ASSERT_EQUAL(75, int(round(head->biYPelsPerMeter * INCH)));
}

void TestQtImageLoader::testMultiPage()
{
    QtImageLoader loader;

    if(loader.supportsFormat(FORMAT_PDF)) {
        ASSERT_LOAD_PAGE(loader, "test.pdf", 0);
        ASSERT_LOAD_PAGE(loader, "test.pdf", 1);
    }

    if(loader.supportsFormat(FORMAT_TIFF)) {
        ASSERT_LOAD_PAGE(loader, "multipage.tif", 0);
        ASSERT_LOAD_PAGE(loader, "multipage.tif", 1);
    }
}

void TestQtImageLoader::testSupportedFormats()
{
    QtImageLoader loader;

    FORMAT_SUPPORTED(loader, FORMAT_BMP);
    FORMAT_SUPPORTED(loader, FORMAT_GIF);
    FORMAT_SUPPORTED(loader, FORMAT_JPEG);
    FORMAT_SUPPORTED(loader, FORMAT_PNG);

#ifdef WITH_TIFF
    FORMAT_SUPPORTED(loader, FORMAT_TIFF);
#endif

#ifdef WITH_PDF
    FORMAT_SUPPORTED(loader, FORMAT_PDF);
#endif
}

void TestQtImageLoader::testLoadStream()
{
    std::ifstream fs;
    CPPUNIT_ASSERT_THROW(QtImageLoader().load(fs), QtImageLoader::Exception);
}

void TestQtImageLoader::testImageTypes()
{
    QImage img(QSize(10, 10), QImage::Format_RGB32);
    img.fill(Qt::green);

    QtImageLoader loader;

#define ASSERT_LOAD_QIMAGE(loader, image) CPPUNIT_ASSERT_NO_THROW(loader.fromQImage(image))

    ASSERT_LOAD_QIMAGE(loader, img);

    img.convertToFormat(QImage::Format_RGB16);
    ASSERT_LOAD_QIMAGE(loader, img);
    img.convertToFormat(QImage::Format_RGB888);
    ASSERT_LOAD_QIMAGE(loader, img);
    img.convertToFormat(QImage::Format_RGB666);
    ASSERT_LOAD_QIMAGE(loader, img);
    img.convertToFormat(QImage::Format_RGB555);
    ASSERT_LOAD_QIMAGE(loader, img);
    img.convertToFormat(QImage::Format_RGB444);
    ASSERT_LOAD_QIMAGE(loader, img);
    img.convertToFormat(QImage::Format_Indexed8);
    ASSERT_LOAD_QIMAGE(loader, img);
    img.convertToFormat(QImage::Format_MonoLSB);
    ASSERT_LOAD_QIMAGE(loader, img);
    img.convertToFormat(QImage::Format_Mono);
    ASSERT_LOAD_QIMAGE(loader, img);

#undef ASSERT_LOAD_QIMAGE
}

