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

CPPUNIT_TEST_SUITE_REGISTRATION(TestCTDIB);

void TestCTDIB::testInit()
{
    cf::CTDIB image;
    CPPUNIT_ASSERT(image.createBegin(10, 20, 24));
    CPPUNIT_ASSERT(image.createEnd());
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
