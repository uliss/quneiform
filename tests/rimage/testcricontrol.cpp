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

#include <memory>
#include <cstring>

#include "testcricontrol.h"
#include "rdib/bmpimageloader.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCRIControl);

#include "rimage/cricontrol.h"
#include "rdib/ctdib.h"
#include "cimage/cticontrol.h"
#include "../cimage/test_cimage_common.h"

#ifndef LOADER_TEST_IMAGE_DIR
#define LOADER_TEST_IMAGE_DIR ""
#endif

static const int EXAMPLE_W = 100;
static const int EXAMPLE_H = 100;

using namespace cf;

void TestCRIControl::testInit()
{
    CRIControl ctrl;
}

void TestCRIControl::testBinarize()
{
    DibPtr img_1bit("1bit.bmp");
    CImage::instance().addImage("1bit", img_1bit.handle());

    CRIControl rimage;
    CPPUNIT_ASSERT(!rimage.binarise("1bit", "bin", BINARIZATOR_KRONROD));

    DibPtr img_gray("gray.bmp");
    CImage::instance().addImage("gray", img_gray.handle());
    CPPUNIT_ASSERT(rimage.binarise("gray", "bin", BINARIZATOR_KRONROD));
    CImage::instance().dumpImage("bin", "rimage_binarize_gray.bmp");

    DibPtr color_24("color_24.bmp");
    CImage::instance().addImage("color_24", color_24.handle());
    CPPUNIT_ASSERT(rimage.binarise("color_24", "bin", BINARIZATOR_KRONROD));
    CImage::instance().dumpImage("bin", "rimage_binarize_color_24.bmp");

//    DibPtr color_32("color_32.bmp");
//    CImage::instance().addImage("color_32", color_32.handle());
//    CPPUNIT_ASSERT(rimage.binarise("color_32", "bin", BINARIZATOR_KRONROD));
//    CImage::instance().dumpImage("bin", "rimage_binarize_color_32.bmp");

    DibPtr color_16("color_16.bmp");
    CImage::instance().addImage("color_16", color_16.handle());
    CPPUNIT_ASSERT(!rimage.binarise("color_16", "bin", BINARIZATOR_KRONROD));

    DibPtr color_16b("color_16b.bmp");
    CImage::instance().addImage("color_16b", color_16b.handle());
    CPPUNIT_ASSERT(rimage.binarise("color_16b", "bin", BINARIZATOR_KRONROD));
    CImage::instance().dumpImage("bin", "rimage_binarize_color_16b.bmp");

    DibPtr color_index("color_index.bmp");
    CImage::instance().addImage("color_index", color_index.handle());
    CPPUNIT_ASSERT(rimage.binarise("color_index", "bin", BINARIZATOR_KRONROD));
    CImage::instance().dumpImage("bin", "rimage_binarize_color_index.bmp");
}
