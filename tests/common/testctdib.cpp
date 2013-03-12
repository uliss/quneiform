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
    CPPUNIT_ASSERT(!image.palleteColor(0, NULL));
    CPPUNIT_ASSERT_EQUAL(size_t(0), image.pixelCount());
    CPPUNIT_ASSERT(!image.imageData());
    CPPUNIT_ASSERT(!const_cast<const CTDIB&>(image).imageData());
    CPPUNIT_ASSERT_EQUAL(cf::CTDIB::VERSION_UNKNOWN, image.version());
    CPPUNIT_ASSERT(image.whitePixel() == 0);
    CPPUNIT_ASSERT(image.blackPixel() == 0);

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
   CPPUNIT_ASSERT(image.createEnd());
   CPPUNIT_ASSERT(image.fill(cf::RGBQuad::white()));

   for(int x = 0; x < 10; x++) {
       for(int y = 0; y < 10; y++) {
           cf::RGBQuad tmpc;
           CPPUNIT_ASSERT(image.pixelColor(x, y, &tmpc));
           CPPUNIT_ASSERT(tmpc == cf::RGBQuad::white());
       }
   }

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
           CPPUNIT_ASSERT(tmpc == cf::RGBQuad::green());
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
