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

#ifndef TESTCTDIB_H
#define TESTCTDIB_H

#include <cppunit/extensions/HelperMacros.h>

class TestCTDIB : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestCTDIB);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testApplyMask1);
    CPPUNIT_TEST(testApplyMask4);
    CPPUNIT_TEST(testApplyMask8);
    CPPUNIT_TEST(testApplyMask16);
    CPPUNIT_TEST(testApplyMask24);
    CPPUNIT_TEST(testApplyMask32);
    CPPUNIT_TEST(testSaveToBMP);
    CPPUNIT_TEST(testFill);
    CPPUNIT_TEST(testMapTo24);
    CPPUNIT_TEST(testMapTo32);
    CPPUNIT_TEST(testSetPixelColor);
    CPPUNIT_TEST(testWhitePixel);
    CPPUNIT_TEST(testBlackPixel);
    CPPUNIT_TEST(testResolutionInch);
    CPPUNIT_TEST(testResolutionMeter);
    CPPUNIT_TEST(testLineAt);
    CPPUNIT_TEST(testBitmap);
    CPPUNIT_TEST(testPixelAt);
    CPPUNIT_TEST(testPixelColorIndex);
    CPPUNIT_TEST(testSetPalleteColor);
    CPPUNIT_TEST(testPixelColor);
    CPPUNIT_TEST(testSizes);
    CPPUNIT_TEST(testCopyPallete);
    CPPUNIT_TEST_SUITE_END();
public:
    void testInit();
    void testApplyMask1();
    void testApplyMask4();
    void testApplyMask8();
    void testApplyMask16();
    void testApplyMask24();
    void testApplyMask32();
    void testSaveToBMP();
    void testFill();
    void testMapTo32();
    void testMapTo24();
    void testSetPixelColor();
    void testWhitePixel();
    void testBlackPixel();
    void testResolutionInch();
    void testResolutionMeter();
    void testLineAt();
    void testBitmap();
    void testPixelAt();
    void testPixelColorIndex();
    void testSetPalleteColor();
    void testPixelColor();
    void testSizes();
    void testCopyPallete();
};

#endif // TESTCTDIB_H
