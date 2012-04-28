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

#include "testthresholdbinarizator.h"
#include "rimage/thresholdbinarizator.h"
#include "cimage/cticontrol.h"
#include "../cimage/test_cimage_common.h"

#ifndef LOADER_TEST_IMAGE_DIR
#define LOADER_TEST_IMAGE_DIR ""
#endif

CPPUNIT_TEST_SUITE_REGISTRATION(TestThresholdBinarizator);

void TestThresholdBinarizator::testInit()
{
    cf::ThresholdBinarizator b;
    CPPUNIT_ASSERT_EQUAL(0, b.threshold());

    b.setThreshold(127);
    CPPUNIT_ASSERT_EQUAL(127, b.threshold());
}

void TestThresholdBinarizator::testBinarizeGray()
{
    cf::ThresholdBinarizator b;
    b.setThreshold(200);
    DibPtr img("gray.bmp");
    b.setSource(img);

    CTDIB * result = b.binarize(0);
    CPPUNIT_ASSERT(result);

    cf::CTIControl::writeDIBtoBMP("rimage_threshold_binarize_gray.bmp", result);

    IS_BLACK_DIB1(result, 0, 0);
    IS_BLACK_DIB1(result, 99, 0);
    IS_BLACK_DIB1(result, 0, 99);
    IS_WHITE_DIB1(result, 50, 0);
    IS_WHITE_DIB1(result, 99, 50);

    delete result;
}

void TestThresholdBinarizator::testBinarize24()
{
    cf::ThresholdBinarizator b;
    b.setThreshold(200);
    DibPtr img("color_24.bmp");
    b.setSource(img);

    CTDIB * result = b.binarize(0);
    CPPUNIT_ASSERT(result);

    cf::CTIControl::writeDIBtoBMP("rimage_threshold_binarize_color_24.bmp", result);

    IS_BLACK_DIB1(result, 0, 0);
    IS_BLACK_DIB1(result, 99, 0);
    IS_BLACK_DIB1(result, 0, 99);
    IS_WHITE_DIB1(result, 50, 0);
    IS_WHITE_DIB1(result, 99, 50);

    delete result;
}
