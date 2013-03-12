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

#include "testctdib.h"
#include "common/ctdib.h"
#include "common/bitmask.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCTDIB);

using namespace cf;

void TestCTDIB::testInit()
{
    cf::CTDIB image;
    CPPUNIT_ASSERT(!image.reset());
    CPPUNIT_ASSERT(!image.pixelAt(0, 0));
    CPPUNIT_ASSERT(!image.bpp());
    CPPUNIT_ASSERT(!image.width());
    CPPUNIT_ASSERT(!image.height());
    CPPUNIT_ASSERT(!image.linesNumber());
    CPPUNIT_ASSERT(!image.lineWidth());
    CPPUNIT_ASSERT(!image.lineRealWidthInBytes());
    CPPUNIT_ASSERT(!image.lineWidthInBytes());
    CPPUNIT_ASSERT(!image.imageSizeInBytes());
    CPPUNIT_ASSERT(!image.palleteUsedColorsNum());
    CPPUNIT_ASSERT(!image.header());
    CPPUNIT_ASSERT(!image.pallete());
    CPPUNIT_ASSERT(!image.dibSize());
    CPPUNIT_ASSERT(!image.palleteSize());
    CPPUNIT_ASSERT(!image.headerSize());
    CPPUNIT_ASSERT(!image.palleteColor(0, NULL));
    CPPUNIT_ASSERT_EQUAL(size_t(0), image.pixelCount());
    CPPUNIT_ASSERT(!image.imageData());
    CPPUNIT_ASSERT(!const_cast<const CTDIB&>(image).imageData());
    CPPUNIT_ASSERT_EQUAL(cf::CTDIB::VERSION_UNKNOWN, image.version());
    CPPUNIT_ASSERT(image.whitePixel() == 0);
    CPPUNIT_ASSERT(image.blackPixel() == 0);

    CPPUNIT_ASSERT(!image.createBegin(10, 10, 24, (CTDIB::version_t) 1024));

    CPPUNIT_ASSERT(image.createBegin(10, 20, 24));
    CPPUNIT_ASSERT(image.createEnd());

    CPPUNIT_ASSERT(image.whitePixel() == 0x00FFFFFF);
    CPPUNIT_ASSERT(image.blackPixel() == 0);
    CPPUNIT_ASSERT_EQUAL(size_t(10 * 20), image.pixelCount());
    CPPUNIT_ASSERT(image.imageData());
    CPPUNIT_ASSERT(const_cast<const CTDIB&>(image).imageData());
    CPPUNIT_ASSERT(image.version() == cf::CTDIB::VERSION_WINDOWS);
    CPPUNIT_ASSERT(image.header()->biBitCount == 24);
    CPPUNIT_ASSERT(image.header()->biSize == 40);
    CPPUNIT_ASSERT(image.saveToBMP("test_rdib.bmp"));

    for(int i = 0; i < image.lineWidth(); i++) {
        uchar * p = (uchar*) image.pixelAt(i, 0);
        p[0] = 0xff;
        p[1] = 0xff;
        p[2] = 0x00;
    }

    CPPUNIT_ASSERT(image.saveToBMP("test_rdib_draw.bmp"));
}

void TestCTDIB::testApplyMask1()
{
    CTDIB image;
    BitMask mask(3, 3);

    CPPUNIT_ASSERT(!image.applyMask(mask));

    image.createBegin(3, 3, 1);
    image.makeBlackAndWhitePallete();
    image.createEnd();
    image.fill(RGBQuad::black());

    BitMask m1(3, 1);
    CPPUNIT_ASSERT(!image.applyMask(m1));
    BitMask m2(9, 3);
    CPPUNIT_ASSERT(!image.applyMask(m2));

#define IS_BLACK(image, x, y) {\
    RGBQuad c;\
    CPPUNIT_ASSERT(image.pixelColor(x, y, &c));\
    CPPUNIT_ASSERT_EQUAL(RGBQuad::black(), c);\
}

#define IS_WHITE(image, x, y) {\
    RGBQuad c;\
    CPPUNIT_ASSERT(image.pixelColor(x, y, &c));\
    CPPUNIT_ASSERT_EQUAL(RGBQuad::white(), c);\
}

    IS_BLACK(image, 0, 0);
    IS_BLACK(image, 1, 0);
    IS_BLACK(image, 2, 0);
    IS_BLACK(image, 0, 1);
    IS_BLACK(image, 1, 1);
    IS_BLACK(image, 2, 1);
    IS_BLACK(image, 0, 2);
    IS_BLACK(image, 1, 2);
    IS_BLACK(image, 2, 2);

    CPPUNIT_ASSERT(image.applyMask(mask));

    IS_WHITE(image, 0, 0);
    IS_WHITE(image, 1, 0);
    IS_WHITE(image, 2, 0);
    IS_WHITE(image, 0, 1);
    IS_WHITE(image, 1, 1);
    IS_WHITE(image, 2, 1);
    IS_WHITE(image, 0, 2);
    IS_WHITE(image, 1, 2);
    IS_WHITE(image, 2, 2);
    image.fill(RGBQuad::black());

    mask.fillRect(Rect(1, 1, 3, 3), true);
    CPPUNIT_ASSERT(image.applyMask(mask));

    IS_WHITE(image, 0, 0);
    IS_WHITE(image, 1, 0);
    IS_WHITE(image, 2, 0);
    IS_WHITE(image, 0, 1);
    IS_BLACK(image, 1, 1);
    IS_BLACK(image, 2, 1);
    IS_WHITE(image, 0, 2);
    IS_BLACK(image, 1, 2);
    IS_BLACK(image, 2, 2);

    image.setPalleteColor(0, RGBQuad::white());
    image.setPalleteColor(1, RGBQuad::black());
    image.fill(RGBQuad::black());
    IS_BLACK(image, 0, 0);

    CPPUNIT_ASSERT(image.applyMask(mask));

    IS_WHITE(image, 0, 0);
    IS_WHITE(image, 1, 0);
    IS_WHITE(image, 2, 0);
    IS_WHITE(image, 0, 1);
    IS_BLACK(image, 1, 1);
    IS_BLACK(image, 2, 1);
    IS_WHITE(image, 0, 2);
    IS_BLACK(image, 1, 2);
    IS_BLACK(image, 2, 2);
}

void TestCTDIB::testApplyMask4()
{
    CTDIB image;
    image.createBegin(3, 3, 4);
    image.createEnd();

    BitMask mask(3, 3);
    CPPUNIT_ASSERT(!image.applyMask(mask));
}

void TestCTDIB::testApplyMask8()
{
    CTDIB image;
    image.createBegin(3, 3, 8);
    image.addPalleteColor(RGBQuad::black());
    image.addPalleteColor(RGBQuad::green());
    image.addPalleteColor(RGBQuad::white());
    image.createEnd();
    CPPUNIT_ASSERT(image.fill(RGBQuad::black()));

    IS_BLACK(image, 0, 0);
    IS_BLACK(image, 1, 0);
    IS_BLACK(image, 2, 0);
    IS_BLACK(image, 0, 1);
    IS_BLACK(image, 1, 1);
    IS_BLACK(image, 2, 1);
    IS_BLACK(image, 0, 2);
    IS_BLACK(image, 1, 2);
    IS_BLACK(image, 2, 2);

    BitMask mask(3, 3);
    CPPUNIT_ASSERT(image.applyMask(mask));

    IS_WHITE(image, 0, 0);
    IS_WHITE(image, 1, 0);
    IS_WHITE(image, 2, 0);
    IS_WHITE(image, 0, 1);
    IS_WHITE(image, 1, 1);
    IS_WHITE(image, 2, 1);
    IS_WHITE(image, 0, 2);
    IS_WHITE(image, 1, 2);
    IS_WHITE(image, 2, 2);

    image.fill(RGBQuad::green());
    mask.fillRect(Rect(1, 1, 3, 3), true);
    CPPUNIT_ASSERT(image.applyMask(mask));

#define IS_GREEN(image, x, y) {\
    RGBQuad c;\
    CPPUNIT_ASSERT(image.pixelColor(x, y, &c));\
    CPPUNIT_ASSERT_EQUAL(RGBQuad::green(), c);\
}

    IS_WHITE(image, 0, 0);
    IS_WHITE(image, 1, 0);
    IS_WHITE(image, 2, 0);
    IS_WHITE(image, 0, 1);
    IS_GREEN(image, 1, 1);
    IS_GREEN(image, 2, 1);
    IS_WHITE(image, 0, 2);
    IS_GREEN(image, 1, 2);
    IS_GREEN(image, 2, 2);
}

void TestCTDIB::testApplyMask16()
{
    CTDIB image;
    image.createBegin(3, 3, 16);
    image.createEnd();

    BitMask mask(3, 3);
    CPPUNIT_ASSERT(!image.applyMask(mask));
}

void TestCTDIB::testApplyMask24()
{
    CTDIB image;
    image.createBegin(3, 3, 24);
    image.createEnd();
    image.fill(RGBQuad::red());

    BitMask mask(3, 3);
    CPPUNIT_ASSERT(image.applyMask(mask));

    IS_WHITE(image, 0, 0);
    IS_WHITE(image, 1, 0);
    IS_WHITE(image, 2, 0);
    IS_WHITE(image, 0, 1);
    IS_WHITE(image, 1, 1);
    IS_WHITE(image, 2, 1);
    IS_WHITE(image, 0, 2);
    IS_WHITE(image, 1, 2);
    IS_WHITE(image, 2, 2);

#define IS_RED(image, x, y) {\
    RGBQuad c;\
    CPPUNIT_ASSERT(image.pixelColor(x, y, &c));\
    CPPUNIT_ASSERT_EQUAL(RGBQuad::red(), c);\
}

    image.fill(RGBQuad::red());
    IS_RED(image, 0, 0);

    mask.fillRect(Rect(1, 1, 2, 2), true);
    CPPUNIT_ASSERT(image.applyMask(mask));

    IS_WHITE(image, 0, 0);
    IS_WHITE(image, 1, 0);
    IS_WHITE(image, 2, 0);
    IS_WHITE(image, 0, 1);
    IS_RED(image, 1, 1);
    IS_RED(image, 2, 1);
    IS_WHITE(image, 0, 2);
    IS_RED(image, 1, 2);
    IS_RED(image, 2, 2);
}

void TestCTDIB::testApplyMask32()
{
    CTDIB image;
    image.createBegin(3, 3, 32);
    image.createEnd();
    image.fill(RGBQuad::red());

    BitMask mask(3, 3);
    CPPUNIT_ASSERT(image.applyMask(mask));

    IS_WHITE(image, 0, 0);
    IS_WHITE(image, 1, 0);
    IS_WHITE(image, 2, 0);
    IS_WHITE(image, 0, 1);
    IS_WHITE(image, 1, 1);
    IS_WHITE(image, 2, 1);
    IS_WHITE(image, 0, 2);
    IS_WHITE(image, 1, 2);
    IS_WHITE(image, 2, 2);

#define IS_RED(image, x, y) {\
    RGBQuad c;\
    CPPUNIT_ASSERT(image.pixelColor(x, y, &c));\
    CPPUNIT_ASSERT_EQUAL(RGBQuad::red(), c);\
}

    image.fill(RGBQuad::red());
    IS_RED(image, 0, 0);

    mask.fillRect(Rect(1, 1, 2, 2), true);
    CPPUNIT_ASSERT(image.applyMask(mask));

    IS_WHITE(image, 0, 0);
    IS_WHITE(image, 1, 0);
    IS_WHITE(image, 2, 0);
    IS_WHITE(image, 0, 1);
    IS_RED(image, 1, 1);
    IS_RED(image, 2, 1);
    IS_WHITE(image, 0, 2);
    IS_RED(image, 1, 2);
    IS_RED(image, 2, 2);
}

void TestCTDIB::testSaveToBMP()
{
    cf::CTDIB image;
    CPPUNIT_ASSERT(!image.saveToBMP("test_rdib_save.bmp"));

    CPPUNIT_ASSERT(image.createBegin(10, 20, 24));
    CPPUNIT_ASSERT(image.createEnd());

    image.fill(cf::RGBQuad::red());
    CPPUNIT_ASSERT(image.saveToBMP("test_rdib_save.bmp"));

    CPPUNIT_ASSERT(!cf::CTDIB::saveToBMP(std::cerr, NULL));
}

void TestCTDIB::testFill()
{
   cf::CTDIB image;
   CPPUNIT_ASSERT(!image.fill(cf::RGBQuad::red()));

   // 1 bit
   CPPUNIT_ASSERT(image.createBegin(10, 10, 1));
   image.makeBlackAndWhitePallete();
   CPPUNIT_ASSERT(image.createEnd());
   CPPUNIT_ASSERT(image.fill(cf::RGBQuad::white()));

   for(int x = 0; x < 10; x++) {
       for(int y = 0; y < 10; y++) {
           cf::RGBQuad tmpc;
           CPPUNIT_ASSERT(image.pixelColor(x, y, &tmpc));
           CPPUNIT_ASSERT(tmpc == cf::RGBQuad::white());
       }
   }

   image.setPixelColor(0, 0, RGBQuad::white());
   image.setPixelColor(0, 1, RGBQuad::black());
   CPPUNIT_ASSERT(!image.fill(RGBQuad::red()));

   // 4 bit
   CPPUNIT_ASSERT(image.reset());
   CPPUNIT_ASSERT(image.createBegin(10, 10, 4));
   CPPUNIT_ASSERT(image.createEnd());
   CPPUNIT_ASSERT(image.fill(cf::RGBQuad::red()));

   for(int x = 0; x < 10; x++) {
       for(int y = 0; y < 10; y++) {
           cf::RGBQuad tmpc;
           CPPUNIT_ASSERT(image.pixelColor(x, y, &tmpc));
           CPPUNIT_ASSERT(tmpc == cf::RGBQuad::red());
       }
   }

   // 8 bit
   CPPUNIT_ASSERT(image.reset());
   CPPUNIT_ASSERT(image.createBegin(10, 10, 8));
   CPPUNIT_ASSERT(image.createEnd());
   CPPUNIT_ASSERT(image.fill(cf::RGBQuad::blue()));

   for(int x = 0; x < 10; x++) {
       for(int y = 0; y < 10; y++) {
           cf::RGBQuad tmpc;
           CPPUNIT_ASSERT(image.pixelColor(x, y, &tmpc));
           CPPUNIT_ASSERT(tmpc == cf::RGBQuad::blue());
       }
   }

   // 24 bit
   CPPUNIT_ASSERT(image.reset());
   CPPUNIT_ASSERT(image.createBegin(10, 10, 24));
   CPPUNIT_ASSERT(image.createEnd());
   CPPUNIT_ASSERT(image.fill(cf::RGBQuad::green()));

   for(int x = 0; x < 10; x++) {
       for(int y = 0; y < 10; y++) {
           cf::RGBQuad tmpc;
           CPPUNIT_ASSERT(image.pixelColor(x, y, &tmpc));
           CPPUNIT_ASSERT_EQUAL(RGBQuad::green(), tmpc);
       }
   }

   // 32 bit
   CPPUNIT_ASSERT(image.reset());
   CPPUNIT_ASSERT(image.createBegin(10, 10, 32));
   CPPUNIT_ASSERT(image.createEnd());
   CPPUNIT_ASSERT(image.fill(cf::RGBQuad::blue()));

   for(int x = 0; x < 10; x++) {
       for(int y = 0; y < 10; y++) {
           cf::RGBQuad tmpc;
           CPPUNIT_ASSERT(image.pixelColor(x, y, &tmpc));
           CPPUNIT_ASSERT(tmpc == cf::RGBQuad::blue());
       }
   }

}

static std::vector<cf::RGBQuad> test_map_32_vector;
static std::vector<uint> test_map_32_vector_x;
static std::vector<uint> test_map_32_vector_y;
void testMap32Function(cf::RGBQuad * pixel, uint x, uint y)
{
    test_map_32_vector.push_back(*pixel);
    test_map_32_vector_x.push_back(x);
    test_map_32_vector_y.push_back(y);
}

void testMap32FunctionConst(const cf::RGBQuad * pixel, uint x, uint y)
{
    test_map_32_vector.push_back(*pixel);
    test_map_32_vector_x.push_back(x);
    test_map_32_vector_y.push_back(y);
}

void TestCTDIB::testMapTo32()
{
    const RGBQuad gray(127, 127, 127);
    CTDIB image;
    image.createBegin(4, 4, 32);
    image.createEnd();
    image.fill(gray);
    image.setPixelColor(0, 0, RGBQuad::white());
    RGBQuad c;
    image.pixelColor(0, 0, &c);
    CPPUNIT_ASSERT(c == RGBQuad::white());

    CPPUNIT_ASSERT_EQUAL(size_t(16), image.pixelCount());

    CPPUNIT_ASSERT(test_map_32_vector.empty());
    image.mapToPixels32(testMap32Function);
    CPPUNIT_ASSERT_EQUAL(size_t(16), test_map_32_vector.size());
    CPPUNIT_ASSERT_EQUAL(size_t(16), test_map_32_vector_x.size());
    CPPUNIT_ASSERT_EQUAL(size_t(16), test_map_32_vector_y.size());

    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_x[0]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_x[1]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_x[2]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_x[3]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_x[4]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_x[5]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_x[6]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_x[7]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_x[8]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_x[9]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_x[10]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_x[11]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_x[12]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_x[13]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_x[14]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_x[15]);

    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_y[0]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_y[1]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_y[2]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_y[3]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_y[4]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_y[5]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_y[6]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_y[7]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_y[8]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_y[9]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_y[10]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_y[11]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_y[12]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_y[13]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_y[14]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_y[15]);

    // NOTE: line order is bottom to top
    CPPUNIT_ASSERT(test_map_32_vector[11] == gray);
    CPPUNIT_ASSERT(test_map_32_vector[12] == RGBQuad::white());
    CPPUNIT_ASSERT(test_map_32_vector[13] == gray);

    CTDIB empty;
    empty.mapToPixels32(&testMap32Function);
    const_cast<const CTDIB&>(empty).mapToPixels32(&testMap32FunctionConst);

    test_map_32_vector.clear();
    test_map_32_vector_x.clear();
    test_map_32_vector_y.clear();

    const_cast<const CTDIB&>(image).mapToPixels32(&testMap32FunctionConst);
    CPPUNIT_ASSERT_EQUAL(size_t(16), test_map_32_vector.size());
    CPPUNIT_ASSERT_EQUAL(size_t(16), test_map_32_vector_x.size());
    CPPUNIT_ASSERT_EQUAL(size_t(16), test_map_32_vector_y.size());

    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_x[0]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_x[1]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_x[2]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_x[3]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_x[4]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_x[5]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_x[6]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_x[7]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_x[8]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_x[9]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_x[10]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_x[11]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_x[12]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_x[13]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_x[14]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_x[15]);

    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_y[0]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_y[1]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_y[2]);
    CPPUNIT_ASSERT_EQUAL(0, (int) test_map_32_vector_y[3]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_y[4]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_y[5]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_y[6]);
    CPPUNIT_ASSERT_EQUAL(1, (int) test_map_32_vector_y[7]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_y[8]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_y[9]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_y[10]);
    CPPUNIT_ASSERT_EQUAL(2, (int) test_map_32_vector_y[11]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_y[12]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_y[13]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_y[14]);
    CPPUNIT_ASSERT_EQUAL(3, (int) test_map_32_vector_y[15]);

    // NOTE: line order is bottom to top
    CPPUNIT_ASSERT(test_map_32_vector[11] == gray);
    CPPUNIT_ASSERT(test_map_32_vector[12] == RGBQuad::white());
    CPPUNIT_ASSERT(test_map_32_vector[13] == gray);

}

struct Map24Tester
{
    static void clear()
    {
        data.clear();
        data_x.clear();
        data_y.clear();
    }

    static void f(uchar * pixel, uint x, uint y)
    {
        RGBQuad c;
        c.rgbBlue = pixel[0];
        c.rgbGreen = pixel[1];
        c.rgbRed = pixel[2];
        data.push_back(c);
        data_x.push_back(x);
        data_y.push_back(y);
    }

    static void fConst(const uchar * pixel, uint x, uint y)
    {
        RGBQuad c;
        c.rgbBlue = pixel[0];
        c.rgbGreen = pixel[1];
        c.rgbRed = pixel[2];
        data.push_back(c);
        data_x.push_back(x);
        data_y.push_back(y);
    }

    static size_t size() { return data.size(); }
    static std::vector<RGBQuad> data;
    static std::vector<uint> data_x;
    static std::vector<uint> data_y;
};

std::vector<RGBQuad> Map24Tester::data;
std::vector<uint> Map24Tester::data_x;
std::vector<uint> Map24Tester::data_y;

void TestCTDIB::testMapTo24()
{
    const RGBQuad gray(127, 127, 127);
    CTDIB image;
    image.createBegin(4, 4, 24);
    image.createEnd();
    image.fill(gray);
    image.setPixelColor(0, 0, RGBQuad::white());
    RGBQuad c;
    image.pixelColor(0, 0, &c);
    CPPUNIT_ASSERT(c == RGBQuad::white());

    CPPUNIT_ASSERT_EQUAL(size_t(16), image.pixelCount());

    CPPUNIT_ASSERT(Map24Tester::size() == 0);
    CPPUNIT_ASSERT(Map24Tester::data_y.size() == 0);
    CPPUNIT_ASSERT(Map24Tester::data_y.size() == 0);
    image.mapToPixels24(&Map24Tester::f);
    CPPUNIT_ASSERT(Map24Tester::size() == 16);
    CPPUNIT_ASSERT(Map24Tester::data_x.size() == 16);
    CPPUNIT_ASSERT(Map24Tester::data_y.size() == 16);

    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_x[0]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_x[1]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_x[2]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_x[3]);

    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_x[4]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_x[5]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_x[6]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_x[7]);

    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_x[8]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_x[9]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_x[10]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_x[11]);

    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_x[12]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_x[13]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_x[14]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_x[15]);

    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_y[0]);
    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_y[1]);
    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_y[2]);
    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_y[3]);

    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_y[4]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_y[5]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_y[6]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_y[7]);

    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_y[8]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_y[9]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_y[10]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_y[11]);

    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_y[12]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_y[13]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_y[14]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_y[15]);

    // NOTE: line order is bottom to top
    CPPUNIT_ASSERT(Map24Tester::data[11] == gray);
    CPPUNIT_ASSERT(Map24Tester::data[12] == RGBQuad::white());
    CPPUNIT_ASSERT(Map24Tester::data[13] == gray);

    //
    CTDIB empty;
    empty.mapToPixels24(&Map24Tester::f);
    const_cast<const CTDIB&>(empty).mapToPixels24(&Map24Tester::fConst);

    Map24Tester::clear();
    const_cast<const CTDIB&>(image).mapToPixels24(&Map24Tester::fConst);
    CPPUNIT_ASSERT(Map24Tester::size() == 16);
    CPPUNIT_ASSERT(Map24Tester::data_x.size() == 16);
    CPPUNIT_ASSERT(Map24Tester::data_y.size() == 16);

    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_x[0]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_x[1]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_x[2]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_x[3]);

    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_x[4]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_x[5]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_x[6]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_x[7]);

    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_x[8]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_x[9]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_x[10]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_x[11]);

    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_x[12]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_x[13]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_x[14]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_x[15]);

    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_y[0]);
    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_y[1]);
    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_y[2]);
    CPPUNIT_ASSERT_EQUAL(0, (int) Map24Tester::data_y[3]);

    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_y[4]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_y[5]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_y[6]);
    CPPUNIT_ASSERT_EQUAL(1, (int) Map24Tester::data_y[7]);

    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_y[8]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_y[9]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_y[10]);
    CPPUNIT_ASSERT_EQUAL(2, (int) Map24Tester::data_y[11]);

    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_y[12]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_y[13]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_y[14]);
    CPPUNIT_ASSERT_EQUAL(3, (int) Map24Tester::data_y[15]);

    // NOTE: line order is bottom to top
    CPPUNIT_ASSERT(Map24Tester::data[11] == gray);
    CPPUNIT_ASSERT(Map24Tester::data[12] == RGBQuad::white());
    CPPUNIT_ASSERT(Map24Tester::data[13] == gray);
}

void TestCTDIB::testSetPixelColor()
{
    const RGBQuad gray(127, 127, 127);
    CTDIB image;
    CPPUNIT_ASSERT(!image.setPixelColor(0, 0, RGBQuad::red()));
    image.createBegin(4, 4, 24);
    image.createEnd();
    image.fill(gray);

    RGBQuad c;
    image.pixelColor(0, 0, &c);
    CPPUNIT_ASSERT(c == gray);

    image.setPixelColor(0, 0, RGBQuad::white());
    image.pixelColor(0, 0, &c);
    CPPUNIT_ASSERT(c == RGBQuad::white());

    image.pixelColor(1, 0, &c);
    CPPUNIT_ASSERT(c == gray);

    image.reset();
    image.createBegin(3, 3, 1);
    image.createEnd();
    CPPUNIT_ASSERT(image.setPixelColor(0, 0, RGBQuad::black()));
    CPPUNIT_ASSERT(image.setPixelColor(1, 0, RGBQuad::white()));
    CPPUNIT_ASSERT(!image.setPixelColor(2, 0, RGBQuad::green()));
    CPPUNIT_ASSERT(!image.setPixelColor(4, 0, RGBQuad::white()));

    image.reset();
    image.createBegin(3, 3, 24);
    image.createEnd();
    CPPUNIT_ASSERT(image.setPixelColor(2, 2, RGBQuad::white()));
    CPPUNIT_ASSERT(!image.setPixelColor(20, 20, RGBQuad::white()));

    image.reset();
    image.createBegin(3, 3, 32);
    image.createEnd();
    CPPUNIT_ASSERT(image.setPixelColor(2, 2, RGBQuad::white()));
    CPPUNIT_ASSERT(!image.setPixelColor(20, 20, RGBQuad::white()));

    image.header()->biBitCount = 15;
    CPPUNIT_ASSERT(!image.setPixelColor(2, 2, RGBQuad::white()));
}

void TestCTDIB::testWhitePixel()
{
    CTDIB b1;
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b1.whitePixel());
    b1.createBegin(10, 10, 1);
    b1.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint32_t(1), b1.whitePixel());

    CTDIB b4;
    b4.createBegin(10, 10, 4);
    b4.createEnd();
    b4.setPalleteColor(3, RGBQuad::black());
    b4.setPalleteColor(2, RGBQuad::white());
    b4.setPalleteColor(0, RGBQuad::green());
    b4.setPalleteColor(1, RGBQuad::red());
    CPPUNIT_ASSERT_EQUAL(uint32_t(2), b4.whitePixel());

    CTDIB b8;
    b8.createBegin(10, 10, 8);
    b8.createEnd();
    b8.setPalleteColor(3, RGBQuad::black());
    b8.setPalleteColor(2, RGBQuad::white());
    b8.setPalleteColor(0, RGBQuad::green());
    b8.setPalleteColor(1, RGBQuad::red());
    CPPUNIT_ASSERT_EQUAL(uint32_t(2), b8.whitePixel());

    CTDIB b16;
    b16.createBegin(10, 10, 16);
    b16.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint32_t(0x00007fff), b16.whitePixel());

    CTDIB b24;
    b24.createBegin(10, 10, 24);
    b24.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint32_t(0x00ffffff), b24.whitePixel());

    CTDIB b32;
    b32.createBegin(10, 10, 32);
    b32.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint32_t(0xffffffff), b32.whitePixel());
}

void TestCTDIB::testBlackPixel()
{
    CTDIB b1;
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b1.blackPixel());
    b1.createBegin(10, 10, 1);
    b1.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b1.blackPixel());
    CPPUNIT_ASSERT_EQUAL(uint32_t(1), b1.whitePixel());

    CPPUNIT_ASSERT(!b1.createBegin(20, 20, 24));

    b1.setPalleteColor(1, RGBQuad::black());
    b1.setPalleteColor(0, RGBQuad::white());
    CPPUNIT_ASSERT_EQUAL(uint32_t(1), b1.blackPixel());
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b1.whitePixel());

    b1.setPalleteColor(0, RGBQuad::black());
    b1.setPalleteColor(1, RGBQuad::white());
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b1.blackPixel());
    CPPUNIT_ASSERT_EQUAL(uint32_t(1), b1.whitePixel());

    CTDIB b4;
    b4.createBegin(10, 10, 4);
    b4.createEnd();
    b4.setPalleteColor(3, RGBQuad::black());
    b4.setPalleteColor(0, RGBQuad::white());
    b4.setPalleteColor(2, RGBQuad::green());
    b4.setPalleteColor(1, RGBQuad::red());
    CPPUNIT_ASSERT_EQUAL(uint32_t(3), b4.blackPixel());

    CTDIB b8;
    b8.createBegin(10, 10, 8);
    b8.createEnd();
    b8.setPalleteColor(3, RGBQuad::black());
    b8.setPalleteColor(0, RGBQuad::white());
    b8.setPalleteColor(2, RGBQuad::green());
    b8.setPalleteColor(1, RGBQuad::red());
    CPPUNIT_ASSERT_EQUAL(uint32_t(3), b8.blackPixel());

    CTDIB b16;
    b16.createBegin(10, 10, 16);
    b16.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b16.blackPixel());

    CTDIB b24;
    b24.createBegin(10, 10, 24);
    b24.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b24.blackPixel());

    CTDIB b32;
    b32.createBegin(10, 10, 32);
    b32.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint32_t(0), b32.blackPixel());
}

void TestCTDIB::testResolutionInch()
{
    CTDIB image;
    uint x, y;
    CPPUNIT_ASSERT(!image.setResolutionDotsPerInch(10, 10));
    CPPUNIT_ASSERT(!image.resolutionDotsPerInch(&x, NULL));

    image.createBegin(10, 10, 1);
    CPPUNIT_ASSERT(image.setResolutionDotsPerInch(100));
    CPPUNIT_ASSERT(image.resolutionDotsPerInch(&x, &y));
    CPPUNIT_ASSERT_EQUAL(uint(99), x);
    CPPUNIT_ASSERT_EQUAL(uint(99), y);
    x = 0;
    CPPUNIT_ASSERT(image.resolutionDotsPerInch(&x, NULL));
    CPPUNIT_ASSERT_EQUAL(uint(99), x);
    CPPUNIT_ASSERT(!image.resolutionDotsPerInch(NULL, NULL));
    image.createEnd();

    CPPUNIT_ASSERT(!image.setResolutionDotsPerInch(200));
}

void TestCTDIB::testResolutionMeter()
{
    CTDIB image;
    uint x, y;
    CPPUNIT_ASSERT(!image.setResolutionDotsPerMeter(1000, 1000));
    CPPUNIT_ASSERT(!image.resolutionDotsPerMeter(&x, NULL));

    image.createBegin(10, 10, 1);
    CPPUNIT_ASSERT(image.setResolutionDotsPerMeter(1000));
    CPPUNIT_ASSERT(image.resolutionDotsPerMeter(&x, &y));
    CPPUNIT_ASSERT_EQUAL(uint(1000), x);
    CPPUNIT_ASSERT_EQUAL(uint(1000), y);
    x = 0;
    CPPUNIT_ASSERT(image.resolutionDotsPerMeter(&x, NULL));
    CPPUNIT_ASSERT_EQUAL(uint(1000), x);
    CPPUNIT_ASSERT(!image.resolutionDotsPerMeter(NULL, NULL));

    x = 0;
    y = 0;
    CPPUNIT_ASSERT(image.setResolutionDotsPerMeter(1024, 1025));
    CPPUNIT_ASSERT(image.resolutionDotsPerMeter(&x, &y));
    CPPUNIT_ASSERT_EQUAL(uint(1024), x);
    CPPUNIT_ASSERT_EQUAL(uint(1025), y);

    image.createEnd();

    CPPUNIT_ASSERT(!image.setResolutionDotsPerMeter(200));
}

void TestCTDIB::testLineAt()
{
    CTDIB image;
    CPPUNIT_ASSERT(!image.lineAt(0));

    image.createBegin(3, -3, 24, CTDIB::VERSION_4);
    image.createEnd();

    CPPUNIT_ASSERT(image.lineAt(0));
    CPPUNIT_ASSERT(!image.lineAt(3));

    image.fill(RGBQuad::white());
    uchar * pixel = ((uchar*) image.lineAt(0));
    CPPUNIT_ASSERT_EQUAL(image.imageData(), image.lineAt(0));
    pixel[0] = 0;
    pixel[1] = 0;

    IS_RED(image, 0, 0);
    IS_WHITE(image, 1, 0);
    IS_WHITE(image, 2, 0);
    IS_WHITE(image, 0, 1);
    IS_WHITE(image, 1, 1);
    IS_WHITE(image, 2, 1);
    IS_WHITE(image, 0, 2);
    IS_WHITE(image, 1, 2);
    IS_WHITE(image, 2, 2);

    CTDIB image2;
    image2.createBegin(3, 3, 24);
    image2.createEnd();

    CPPUNIT_ASSERT(image2.imageData() != image2.lineAt(0));

    image2.fill(RGBQuad::white());
    pixel = ((uchar*) image2.lineAt(0));
    pixel[0] = 0;
    pixel[1] = 0;

    IS_RED(image2, 0, 0);
    IS_WHITE(image2, 1, 0);
    IS_WHITE(image2, 2, 0);
    IS_WHITE(image2, 0, 1);
    IS_WHITE(image2, 1, 1);
    IS_WHITE(image2, 2, 1);
    IS_WHITE(image2, 0, 2);
    IS_WHITE(image2, 1, 2);
    IS_WHITE(image2, 2, 2);
}

void TestCTDIB::testBitmap()
{
    BitmapPtr dib = NULL;
    CTDIB image;
    CPPUNIT_ASSERT(!image.bitmap(&dib));
    CPPUNIT_ASSERT(!dib);

    image.createBegin(1, 1, 24, CTDIB::VERSION_5);
    image.createEnd();
    CPPUNIT_ASSERT(image.bitmap(&dib));
    CPPUNIT_ASSERT(dib);
}

void TestCTDIB::testPixelAt()
{
    CTDIB image;
    CPPUNIT_ASSERT(!image.pixelAt(0, 0));
    image.createBegin(10, 10, 24);
    image.createEnd();

    CPPUNIT_ASSERT(!image.pixelAt(10, 0));
    CPPUNIT_ASSERT(!image.pixelAt(0, 10));
    CPPUNIT_ASSERT(!image.pixelAt(10, 10));
    CPPUNIT_ASSERT(image.pixelAt(0, 0));
    CPPUNIT_ASSERT(image.pixelAt(9, 9));
    CPPUNIT_ASSERT(image.pixelAt(0, 9));
    CPPUNIT_ASSERT(image.pixelAt(9, 0));
}

void TestCTDIB::testPixelColorIndex()
{
    uint idx = 0;
    CTDIB image;

    CPPUNIT_ASSERT(!image.makeBlackAndWhitePallete());
    CPPUNIT_ASSERT(!image.pixelColorIndex(0, 0, &idx));

    CPPUNIT_ASSERT(image.createBegin(10, 10, 24));
    CPPUNIT_ASSERT(image.createEnd());
    CPPUNIT_ASSERT(!image.pixelColorIndex(0, 0, &idx));
    CPPUNIT_ASSERT(!image.setPixelColorIndex(0, 0, 1));
    CPPUNIT_ASSERT(image.reset());

    CPPUNIT_ASSERT(image.createBegin(10, 10, 32));
    CPPUNIT_ASSERT(image.createEnd());
    CPPUNIT_ASSERT(!image.pixelColorIndex(0, 0, &idx));
    CPPUNIT_ASSERT(!image.setPixelColorIndex(0, 0, 1));
    CPPUNIT_ASSERT(image.reset());

    CPPUNIT_ASSERT(image.createBegin(10, 10, 16));
    CPPUNIT_ASSERT(image.createEnd());
    CPPUNIT_ASSERT(!image.pixelColorIndex(0, 0, &idx));
    CPPUNIT_ASSERT(!image.setPixelColorIndex(0, 0, 1));
    CPPUNIT_ASSERT(image.reset());

    CPPUNIT_ASSERT(image.createBegin(10, 10, 1));
    CPPUNIT_ASSERT(image.makeBlackAndWhitePallete());
    CPPUNIT_ASSERT(image.createEnd());
    image.fill(RGBQuad::white());
    CPPUNIT_ASSERT(image.pixelColorIndex(0, 0, &idx));
    CPPUNIT_ASSERT_EQUAL(uint(1), idx);
    image.setPixelColor(1, 0, RGBQuad::black());
    CPPUNIT_ASSERT(image.pixelColorIndex(0, 0, &idx));
    CPPUNIT_ASSERT_EQUAL(uint(1), idx);
    CPPUNIT_ASSERT(image.pixelColorIndex(1, 0, &idx));
    CPPUNIT_ASSERT_EQUAL(uint(0), idx);

    CPPUNIT_ASSERT(!image.pixelColorIndex(1, 1, NULL));

    image.reset();
    CPPUNIT_ASSERT(!image.setPixelColorIndex(0, 0, 0));
    image.createBegin(10, 10, 1);
    image.makeBlackAndWhitePallete();
    image.createEnd();
    CPPUNIT_ASSERT(image.setPixelColorIndex(0, 0, 1));
    CPPUNIT_ASSERT(image.setPixelColorIndex(1, 0, 0));
    CPPUNIT_ASSERT(!image.setPixelColorIndex(100, 200, 0));
    IS_WHITE(image, 0, 0);
    IS_BLACK(image, 1, 0);
    CPPUNIT_ASSERT(!image.setPixelColorIndex(1, 1, 23));

    image.reset();
    image.createBegin(2, 2, 4);
    image.setPalleteColor(0, RGBQuad::green());
    image.setPalleteColor(1, RGBQuad::red());
    image.setPalleteColor(2, RGBQuad::white());
    image.setPalleteColor(3, RGBQuad::black());
    image.createEnd();

    image.setPixelColorIndex(0, 0, 0);
    image.setPixelColorIndex(1, 0, 1);
    image.setPixelColorIndex(0, 1, 2);
    image.setPixelColorIndex(1, 1, 3);

    IS_GREEN(image, 0, 0);
    IS_RED(image, 1, 0);
    IS_WHITE(image, 0, 1);
    IS_BLACK(image, 1, 1);
}

void TestCTDIB::testSetPalleteColor()
{
    CTDIB image;
    CPPUNIT_ASSERT(!image.setPalleteColor(0, RGBQuad::green()));
    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::black()) == -1);

    image.createBegin(10, 10, 24);
    image.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint(0), image.palleteUsedColorsNum());
    CPPUNIT_ASSERT(!image.setPalleteColor(0, RGBQuad::green()));
    CPPUNIT_ASSERT_EQUAL(size_t(0), image.palleteSize());
    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::black()) == -1);
    image.reset();

    image.createBegin(10, 10, 32);
    image.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint(0), image.palleteUsedColorsNum());
    CPPUNIT_ASSERT(!image.setPalleteColor(0, RGBQuad::green()));
    CPPUNIT_ASSERT_EQUAL(size_t(0), image.palleteSize());
    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::black()) == -1);
    image.reset();

    image.createBegin(10, 10, 16);
    image.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint(0), image.palleteUsedColorsNum());
    CPPUNIT_ASSERT(!image.setPalleteColor(0, RGBQuad::green()));
    CPPUNIT_ASSERT_EQUAL(size_t(0), image.palleteSize());
    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::black()) == -1);
    image.reset();

    image.createBegin(10, 10, 1);
    image.createEnd();
    CPPUNIT_ASSERT_EQUAL(uint(2), image.palleteUsedColorsNum());
    CPPUNIT_ASSERT(image.setPalleteColor(0, RGBQuad::green()));
    CPPUNIT_ASSERT(image.setPalleteColor(1, RGBQuad::red()));
    CPPUNIT_ASSERT(!image.setPalleteColor(2, RGBQuad::blue()));
    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::black()) == -1);
    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::white()) == -1);

    image.setPixelColorIndex(0, 0, 1);
    IS_RED(image, 0, 0);

    RGBQuad c;
    CPPUNIT_ASSERT(image.palleteColor(0, &c));
    CPPUNIT_ASSERT(image.palleteColor(1, &c));
    CPPUNIT_ASSERT(!image.palleteColor(2, &c));

    CPPUNIT_ASSERT_EQUAL(size_t(8), image.palleteSize());

    image.reset();
    image.createBegin(10, 10, 1);
    image.createEnd();

    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::black()) == 0);
    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::white()) == -1);

    image.reset();
    image.createBegin(10, 10, 1);
    image.makeBlackAndWhitePallete();
    image.createEnd();

    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::black()) == 0);
    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::white()) == 1);

    image.reset();
    image.createBegin(10, 10, 1);
    image.createEnd();

    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::black()) == 0);
    CPPUNIT_ASSERT(image.setPixelColorIndex(0, 0, 0));
    int idx = image.addPalleteColor(RGBQuad::green());
    CPPUNIT_ASSERT(idx == 1);
    CPPUNIT_ASSERT(image.setPixelColorIndex(1, 0, idx));
    CPPUNIT_ASSERT(image.addPalleteColor(RGBQuad::red()) == -1);
    CPPUNIT_ASSERT(image.setPixelColorIndex(2, 0, 0));
    CPPUNIT_ASSERT(image.addPalleteColor(RGBQuad::blue()) == -1);

    image.reset();
    image.createBegin(10, 10, 4);
    image.createEnd();

    CPPUNIT_ASSERT(image.palleteColorIndex(RGBQuad::black()) == 0);
    CPPUNIT_ASSERT(image.setPixelColorIndex(0, 0, 0));
    idx = image.addPalleteColor(RGBQuad::green());
    CPPUNIT_ASSERT(idx == 1);
    CPPUNIT_ASSERT(image.setPixelColorIndex(1, 0, idx));
    CPPUNIT_ASSERT(image.addPalleteColor(RGBQuad::red()) == 2);
    CPPUNIT_ASSERT(image.setPixelColorIndex(2, 0, 2));
    CPPUNIT_ASSERT(image.addPalleteColor(RGBQuad::blue()) == 3);
    CPPUNIT_ASSERT(image.setPixelColorIndex(3, 0, 3));
    CPPUNIT_ASSERT(image.addPalleteColor(RGBQuad::blue()) == 4);
    CPPUNIT_ASSERT(image.setPixelColorIndex(2, 0,  0));
    CPPUNIT_ASSERT(image.addPalleteColor(RGBQuad(1, 2, 3)) == 2);
}

void TestCTDIB::testPixelColor()
{
    RGBQuad c;
    CTDIB image;
    CPPUNIT_ASSERT(!image.pixelColor(0, 0, NULL));

    image.createBegin(3, 3, 32);
    image.createEnd();
    CPPUNIT_ASSERT(image.pixelColor(0, 0, &c));
    CPPUNIT_ASSERT(!image.pixelColor(0, 0, NULL));
    CPPUNIT_ASSERT(!image.pixelColor(100, 3000, &c));

    image.header()->biBitCount = 15;
    CPPUNIT_ASSERT(!image.pixelColor(0, 0, NULL));

    image.reset();
    image.createBegin(3, 3, 24);
    image.createEnd();
    CPPUNIT_ASSERT(image.pixelColor(0, 0, &c));
    CPPUNIT_ASSERT(!image.pixelColor(0, 0, NULL));
    CPPUNIT_ASSERT(!image.pixelColor(100, 3000, &c));

    image.reset();
    image.createBegin(3, 3, 16);
    image.createEnd();
    CPPUNIT_ASSERT(image.pixelColor(0, 0, &c));
    CPPUNIT_ASSERT(!image.pixelColor(0, 0, NULL));
    CPPUNIT_ASSERT(!image.pixelColor(100, 3000, &c));

    image.reset();
    image.createBegin(3, 3, 16);
    image.header()->biCompression = 3;
    image.createEnd();
    CPPUNIT_ASSERT(!image.pixelColor(0, 0, &c));

    image.reset();
    image.createBegin(3, 3, 16, CTDIB::VERSION_4);
    image.header()->biCompression = 3;
    image.createEnd();
    CPPUNIT_ASSERT(!image.pixelColor(0, 0, &c));

    image.reset();
    image.createBegin(3, 3, 8);
    image.makeBlackAndWhitePallete();
    image.createEnd();
    image.setPixelColor(0, 0, RGBQuad::white());
    CPPUNIT_ASSERT(image.pixelColor(0, 0, &c));
    CPPUNIT_ASSERT_EQUAL(RGBQuad::white(), c);
    char * pixel = (char*) image.pixelAt(0, 0);
    pixel[0] = 12;
    CPPUNIT_ASSERT(image.pixelColor(0, 0, &c));
    CPPUNIT_ASSERT_EQUAL(size_t(3), image.lineRealWidthInBytes());
}

void TestCTDIB::testSizes()
{
    CTDIB image;
    CPPUNIT_ASSERT(!image.pixelShiftInByte(1));

    image.createBegin(3, -3, 32);
    image.createEnd();
    CPPUNIT_ASSERT_EQUAL(size_t(0), image.palleteSize());
    CPPUNIT_ASSERT(!image.makeBlackAndWhitePallete());
    CPPUNIT_ASSERT_EQUAL(size_t(3), image.lineWidth());
    CPPUNIT_ASSERT_EQUAL(size_t(3 * 4), image.lineWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(3 * 4), image.lineRealWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(3 * 4 * 3), image.imageSizeInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(DIB_VERSION_3_HEADER_SIZE), image.headerSize());
    CPPUNIT_ASSERT_EQUAL(size_t(40 + 3 * 4 * 3), image.dibSize());
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(0));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(1));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(2));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(3));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(4));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(5));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(6));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(7));

    image.reset();
    image.createBegin(3, -3, 24);
    image.createEnd();
    CPPUNIT_ASSERT_EQUAL(size_t(0), image.palleteSize());
    CPPUNIT_ASSERT_EQUAL(size_t(3), image.lineWidth());
    CPPUNIT_ASSERT_EQUAL(size_t(3 * 4), image.lineWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(3 * 3), image.lineRealWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(3 * 4 * 3), image.imageSizeInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(DIB_VERSION_3_HEADER_SIZE), image.headerSize());
    CPPUNIT_ASSERT_EQUAL(size_t(40 + 3 * 4 * 3), image.dibSize());
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(0));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(1));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(2));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(3));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(4));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(5));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(6));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(7));

    image.reset();
    image.createBegin(3, -3, 16);
    image.createEnd();
    CPPUNIT_ASSERT_EQUAL(size_t(0), image.palleteSize());
    CPPUNIT_ASSERT_EQUAL(size_t(3), image.lineWidth());
    CPPUNIT_ASSERT_EQUAL(size_t(6 + 2 /* padding */), image.lineWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(3 * 2), image.lineRealWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t((6 + 2) * 3), image.imageSizeInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(DIB_VERSION_3_HEADER_SIZE), image.headerSize());
    CPPUNIT_ASSERT_EQUAL(size_t(40 + (6 + 2) * 3), image.dibSize());
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(0));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(1));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(2));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(3));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(4));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(5));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(6));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(7));

    image.reset();
    image.createBegin(3, -3, 8);
    image.createEnd();
    CPPUNIT_ASSERT_EQUAL(size_t(256 * 4), image.palleteSize());
    CPPUNIT_ASSERT_EQUAL(size_t(3), image.lineWidth());
    CPPUNIT_ASSERT_EQUAL(size_t(3 + 1 /* padding */), image.lineWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(3), image.lineRealWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t((3 + 1) * 3), image.imageSizeInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(DIB_VERSION_3_HEADER_SIZE), image.headerSize());
    CPPUNIT_ASSERT_EQUAL(size_t(40 + (3 + 1) * 3 + 1024), image.dibSize());
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(0));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(1));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(2));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(3));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(4));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(5));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(6));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(7));

    image.reset();
    image.createBegin(3, -3, 4);
    image.createEnd();
    CPPUNIT_ASSERT_EQUAL(size_t(16 * 4), image.palleteSize());
    CPPUNIT_ASSERT_EQUAL(size_t(3), image.lineWidth());
    CPPUNIT_ASSERT_EQUAL(size_t(2 + 2 /* padding */), image.lineWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(2), image.lineRealWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t((3 + 1) * 3), image.imageSizeInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(DIB_VERSION_3_HEADER_SIZE), image.headerSize());
    CPPUNIT_ASSERT_EQUAL(size_t(40 + (2 + 2) * 3 + 16 * 4), image.dibSize());
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(0));
    CPPUNIT_ASSERT_EQUAL(uint(4), image.pixelShiftInByte(1));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(2));
    CPPUNIT_ASSERT_EQUAL(uint(4), image.pixelShiftInByte(3));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(4));
    CPPUNIT_ASSERT_EQUAL(uint(4), image.pixelShiftInByte(5));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(6));
    CPPUNIT_ASSERT_EQUAL(uint(4), image.pixelShiftInByte(7));

    image.reset();
    image.createBegin(3, -3, 1);
    image.createEnd();
    CPPUNIT_ASSERT_EQUAL(size_t(2 * 4), image.palleteSize());
    CPPUNIT_ASSERT_EQUAL(size_t(3), image.lineWidth());
    CPPUNIT_ASSERT_EQUAL(size_t(1 + 3 /* padding */), image.lineWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(1), image.lineRealWidthInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t((1 + 3) * 3), image.imageSizeInBytes());
    CPPUNIT_ASSERT_EQUAL(size_t(DIB_VERSION_3_HEADER_SIZE), image.headerSize());
    CPPUNIT_ASSERT_EQUAL(size_t(40 + (2 + 2) * 3 + 2 * 4), image.dibSize());
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(0));
    CPPUNIT_ASSERT_EQUAL(uint(1), image.pixelShiftInByte(1));
    CPPUNIT_ASSERT_EQUAL(uint(2), image.pixelShiftInByte(2));
    CPPUNIT_ASSERT_EQUAL(uint(3), image.pixelShiftInByte(3));
    CPPUNIT_ASSERT_EQUAL(uint(4), image.pixelShiftInByte(4));
    CPPUNIT_ASSERT_EQUAL(uint(5), image.pixelShiftInByte(5));
    CPPUNIT_ASSERT_EQUAL(uint(6), image.pixelShiftInByte(6));
    CPPUNIT_ASSERT_EQUAL(uint(7), image.pixelShiftInByte(7));
    CPPUNIT_ASSERT_EQUAL(uint(0), image.pixelShiftInByte(8));
}

void TestCTDIB::testCopyPallete()
{
    CTDIB img1;
    img1.createBegin(10, 10, 1);
    img1.makeBlackAndWhitePallete();
    img1.createEnd();

    CTDIB img2;
    img2.createBegin(10, 10, 1);
    img2.setPalleteColor(0, RGBQuad::white());
    img2.setPalleteColor(1, RGBQuad::black());
    img2.createEnd();

    CPPUNIT_ASSERT_EQUAL(img2.palleteColorIndex(RGBQuad::white()), img1.palleteColorIndex(RGBQuad::black()));
    CPPUNIT_ASSERT_EQUAL(img1.palleteColorIndex(RGBQuad::white()), img2.palleteColorIndex(RGBQuad::black()));

    CTDIB img3;
    CPPUNIT_ASSERT(!img3.copyPalleteFromDIB(&img2));
    img3.createBegin(20, 40, 1);
    img3.createEnd();

    CPPUNIT_ASSERT(img3.copyPalleteFromDIB(&img1));
    CPPUNIT_ASSERT_EQUAL(img1.palleteColorIndex(RGBQuad::white()), img3.palleteColorIndex(RGBQuad::white()));
    CPPUNIT_ASSERT_EQUAL(img1.palleteColorIndex(RGBQuad::black()), img3.palleteColorIndex(RGBQuad::black()));

    CPPUNIT_ASSERT(img3.copyPalleteFromDIB(&img2));
    CPPUNIT_ASSERT_EQUAL(img2.palleteColorIndex(RGBQuad::white()), img3.palleteColorIndex(RGBQuad::white()));
    CPPUNIT_ASSERT_EQUAL(img2.palleteColorIndex(RGBQuad::black()), img3.palleteColorIndex(RGBQuad::black()));

    CTDIB img4;
    img4.createBegin(20, 40, 4);
    img4.createEnd();
    CPPUNIT_ASSERT(!img3.copyPalleteFromDIB(&img4));
    CPPUNIT_ASSERT(img4.copyPalleteFromDIB(&img3));
    CPPUNIT_ASSERT_EQUAL(img2.palleteColorIndex(RGBQuad::white()), img4.palleteColorIndex(RGBQuad::white()));
    CPPUNIT_ASSERT_EQUAL(img2.palleteColorIndex(RGBQuad::black()), img4.palleteColorIndex(RGBQuad::black()));

    CPPUNIT_ASSERT(!img4.copyPalleteFromDIB(NULL));
}

void TestCTDIB::testCopyDPI()
{
    CTDIB image;
    image.createBegin(10, 40, 24);
    image.setResolutionDotsPerMeter(1024);
    image.createEnd();

    CTDIB image2;
    CTDIB image3;
    CPPUNIT_ASSERT(!image2.copyDPIFromDIB(&image));
    image2.createBegin(1, 2, 1);
    CPPUNIT_ASSERT(!image2.copyDPIFromDIB(NULL));
    CPPUNIT_ASSERT(image2.copyDPIFromDIB(&image));
    CPPUNIT_ASSERT(!image2.copyDPIFromDIB(&image3));
    image2.createEnd();

    uint x, y;
    CPPUNIT_ASSERT(image2.resolutionDotsPerMeter(&x, &y));
    CPPUNIT_ASSERT_EQUAL(uint(1024), x);
    CPPUNIT_ASSERT_EQUAL(uint(1024), y);

    CPPUNIT_ASSERT(!image2.copyDPIFromDIB(&image));
}
