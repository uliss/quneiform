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

#include "testcticontrol.h"
#include "rdib/bmpimageloader.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCTIControl);

#include "cimage/cticontrol.h"
#include "cimage/ctimask.h"
#include "cimage/bitmask.h"
#include "rdib/ctdib.h"
#include "test_cimage_common.h"

#ifndef LOADER_TEST_IMAGE_DIR
#define LOADER_TEST_IMAGE_DIR ""
#endif

static const int EXAMPLE_W = 100;
static const int EXAMPLE_H = 100;

using namespace cf;

void TestCTIControl::testInit()
{
    CTIControl ctrl;
}

void TestCTIControl::testAddImage()
{
    CTIControl ctrl;
    BitmapHandle handle = loadDibFromBmp("black_1.bmp");
    CPPUNIT_ASSERT(handle);
    CPPUNIT_ASSERT(ctrl.addImage("first", handle));
    CPPUNIT_ASSERT(ctrl.removeImage("first"));

    CPPUNIT_ASSERT(ctrl.addImageCopy("second", handle));
    CPPUNIT_ASSERT(ctrl.removeImage("second"));

    delete[] handle;
}

void TestCTIControl::testDumpImage()
{
    CTIControl ctrl;
    CPPUNIT_ASSERT(!ctrl.dumpImage("notexists", "notexists.bmp"));

    BitmapHandle handle = loadDibFromBmp("black_1.bmp");
    CPPUNIT_ASSERT(ctrl.addImage("test", handle));
    CPPUNIT_ASSERT(ctrl.dumpImage("test", "test_dump.bmp"));

    unlink("test_dump.bmp");
    delete[] handle;
}

void TestCTIControl::testAddRectToReadMask()
{
    CTIControl ctrl;
    CPPUNIT_ASSERT(!ctrl.addRectToReadMask("test", Rect()));

    // no mask
    BitmapHandle handle = loadDibFromBmp("black_1.bmp");
    ctrl.addImage("test", handle);
    CPPUNIT_ASSERT(ctrl.enableReadMask("test"));
    CPPUNIT_ASSERT(ctrl.addRectToReadMask("test", Rect(Point(0, 0), Point(50, 50))));

    BitmapHandle h = ctrl.imageCopy("test");
    CPPUNIT_ASSERT(h);
    CPPUNIT_ASSERT(ctrl.addImage("test_mask", h));
    CPPUNIT_ASSERT(ctrl.dumpImage("test_mask", "cimage_add_rect.bmp"));
    IS_WHITE_HANDLE_1(h, 0, 0);
    IS_BLACK_HANDLE_1(h, 50, 50);

    CPPUNIT_ASSERT(ctrl.removeImage("test"));
    ctrl.free(h);

    delete[] handle;
}

void TestCTIControl::testApplyMask()
{
    // 32-bit dib
    {
        DibPtr dib("black_32.bmp");
        CPPUNIT_ASSERT(dib);
        CTIMask mask(EXAMPLE_W, EXAMPLE_H);
        mask.addRectangle(Rect(Point(0, 0), 50, 50));
        mask.removeRectangle(Rect(Point(10, 10), 30, 30));

        CPPUNIT_ASSERT(CTIControl::applyMaskToDIB(dib, &mask, 0, 0));
        IS_WHITE_DIB32(dib, 0, 0);
        IS_BLACK_DIB32(dib, 50, 50);
        CPPUNIT_ASSERT(CTIControl::writeDIBtoBMP("cimage_apply_mask_32.bmp", dib));
    }

    // 24-bit dib
    {
        DibPtr pdib("black_24.bmp");
        CPPUNIT_ASSERT(pdib);
        CTIMask mask(EXAMPLE_W, EXAMPLE_H);
        mask.addRectangle(Rect(Point(0, 0), 50, 50));
        mask.removeRectangle(Rect(Point(10, 10), 30, 30));

        CPPUNIT_ASSERT(CTIControl::applyMaskToDIB(pdib, &mask, 0, 0));
        IS_WHITE_DIB32(pdib, 0, 0);
        IS_BLACK_DIB32(pdib, 50, 50);
        CPPUNIT_ASSERT(CTIControl::writeDIBtoBMP("cimage_apply_mask_24.bmp", pdib));
    }

    // 16-bit dib
    {
        DibPtr pdib("black_16_v1.bmp");
        CPPUNIT_ASSERT(pdib);
        CTIMask mask(EXAMPLE_W, EXAMPLE_H);
        mask.addRectangle(Rect(Point(0, 0), 50, 50));
        mask.removeRectangle(Rect(Point(10, 10), 30, 30));

        CPPUNIT_ASSERT(CTIControl::applyMaskToDIB(pdib, &mask, 0, 0));
        IS_WHITE_DIB32(pdib, 0, 0);
        IS_BLACK_DIB32(pdib, 50, 50);
        CPPUNIT_ASSERT(CTIControl::writeDIBtoBMP("cimage_apply_mask_16_v1.bmp", pdib));
    }

    // 16-bit dib
    {
        DibPtr pdib("black_16_v2.bmp");
        CPPUNIT_ASSERT(pdib);
        CTIMask mask(EXAMPLE_W, EXAMPLE_H);
        mask.addRectangle(Rect(Point(0, 0), 50, 50));
        mask.removeRectangle(Rect(Point(10, 10), 30, 30));

        CPPUNIT_ASSERT(CTIControl::applyMaskToDIB(pdib, &mask, 0, 0));
        IS_WHITE_DIB32(pdib, 0, 0);
        IS_BLACK_DIB32(pdib, 50, 50);
        CPPUNIT_ASSERT(CTIControl::writeDIBtoBMP("cimage_apply_mask_16_v2.bmp", pdib));
    }

    // gray dib
    {
        DibPtr pdib("black_gray.bmp");
        CPPUNIT_ASSERT(pdib);
        CTIMask mask(EXAMPLE_W, EXAMPLE_H);
        mask.addRectangle(Rect(Point(0, 0), 50, 50));
        mask.removeRectangle(Rect(Point(10, 10), 30, 30));

        CPPUNIT_ASSERT(CTIControl::applyMaskToDIB(pdib, &mask, 0, 0));
        IS_WHITE_DIB32(pdib, 0, 0);
        IS_BLACK_DIB32(pdib, 50, 50);
        CPPUNIT_ASSERT(CTIControl::writeDIBtoBMP("cimage_apply_mask_gray.bmp", pdib));
    }

    // 1-bit dib
    {
        DibPtr pdib("black_1.bmp");
        CPPUNIT_ASSERT(pdib);
        CTIMask mask(EXAMPLE_W, EXAMPLE_H);
        mask.addRectangle(Rect(Point(0, 0), 50, 50));
        mask.removeRectangle(Rect(Point(10, 10), 30, 30));

        CPPUNIT_ASSERT(CTIControl::applyMaskToDIB(pdib, &mask, 0, 0));
        IS_WHITE_DIB1(pdib, 0, 0);
        IS_BLACK_DIB1(pdib, 50, 50);
        CPPUNIT_ASSERT(CTIControl::writeDIBtoBMP("cimage_apply_mask_1.bmp", pdib));
    }
}

void TestCTIControl::testImageCopy()
{
    CTIControl ctrl;

    CPPUNIT_ASSERT(!ctrl.imageCopy("unknown"));

    BitmapHandle handle = loadDibFromBmp("black_1.bmp");
    CPPUNIT_ASSERT(ctrl.addImage("test", handle));

    BitmapHandle copy = ctrl.imageCopy("test");
    // default mask
    CPPUNIT_ASSERT(copy);
    CPPUNIT_ASSERT(copy != handle);
    CPPUNIT_ASSERT(ctrl.free(copy));

    ctrl.addRectToReadMask("test", Rect(Point(0, 0), 50, 50));
    copy = ctrl.imageCopy("test");
    IS_WHITE_HANDLE_1(copy, 0, 0);
    IS_BLACK_HANDLE_1(copy, 50, 50);
    CPPUNIT_ASSERT(ctrl.free(copy));

    // disable mask
    ctrl.disableReadMask("test");
    copy = ctrl.imageCopy("test");
    IS_BLACK_HANDLE_1(copy, 0, 0);
    IS_BLACK_HANDLE_1(copy, 50, 50);
    CPPUNIT_ASSERT(ctrl.free(copy));

    // enable mask
    ctrl.enableReadMask("test");
    copy = ctrl.imageCopy("test");
    IS_WHITE_HANDLE_1(copy, 0, 0);
    IS_BLACK_HANDLE_1(copy, 50, 50);
    CPPUNIT_ASSERT(ctrl.free(copy));

    delete[] handle;
}

void TestCTIControl::testCopyToFrame()
{
    DibPtr src_dib("black_1.bmp");
    CPPUNIT_ASSERT(src_dib);

    {
        // not equal image depth
        DibPtr dest_dib("black_gray.bmp");
        CPPUNIT_ASSERT(!CTIControl::copyToDestinationFrame(src_dib, dest_dib, 0, 0, 50));
    }

    {
        // copy area is out of source
        DibPtr dest_dib("black_1.bmp");
        CPPUNIT_ASSERT(!CTIControl::copyToDestinationFrame(src_dib, dest_dib, 200, 200, 50));
    }

    {
        // invalid frame size
        // copy area is less then destination size
        DibPtr dest_dib("black_1.bmp");
        CPPUNIT_ASSERT(!CTIControl::copyToDestinationFrame(src_dib, dest_dib, 10, 10, 50));
    }

    {
        DibPtr dest_dib("white_1.bmp");
        CPPUNIT_ASSERT(CTIControl::copyToDestinationFrame(src_dib, dest_dib, 0, 0, 50));
        CTIControl::writeDIBtoBMP("cimage_copy_to_frame_1.bmp", dest_dib);
    }
}

void TestCTIControl::testCopyFromFrame()
{
    DibPtr src_dib("black_1.bmp");
    CPPUNIT_ASSERT(src_dib);

    {
        // not equal image depth
        DibPtr dest_dib("black_gray.bmp");
        CPPUNIT_ASSERT(!CTIControl::copyFromSourceFrame(src_dib, dest_dib, 0, 50));
    }

    {
        DibPtr dest_dib("white_1.bmp");
        CPPUNIT_ASSERT(CTIControl::copyFromSourceFrame(src_dib, dest_dib, 25, 50));
        CTIControl::writeDIBtoBMP("cimage_copy_from_frame_1.bmp", dest_dib);

        IS_WHITE_DIB1(dest_dib, 0, 0);
        IS_BLACK_DIB1(dest_dib, 0, 50);
        IS_WHITE_DIB1(dest_dib, 0, 99);
    }
}

void TestCTIControl::testGetDIBFromImage()
{
    CTIControl ctrl;
    BitmapHandle dest = NULL;
    CPPUNIT_ASSERT(!ctrl.getDIBFromImage("not-found", Rect(0, 0, 50, 50), NULL, &dest));

    BitmapHandle handle = loadDibFromBmp("black_1.bmp");
    CPPUNIT_ASSERT(handle);
    ctrl.addImage("black_1bit", handle);


    CPPUNIT_ASSERT(ctrl.getDIBFromImage("black_1bit", Rect(0, 0, 50, 50), NULL, &dest));
    CPPUNIT_ASSERT(dest);
    CPPUNIT_ASSERT(dest->biWidth == 50 && dest->biHeight == 50);
    CPPUNIT_ASSERT(ctrl.free(dest));
    dest = NULL;

    // not intersects
    CPPUNIT_ASSERT(ctrl.getDIBFromImage("black_1bit", Rect(-50, -50, 10, 10), NULL, &dest));
    CPPUNIT_ASSERT(dest);
    CPPUNIT_ASSERT(dest->biWidth == 100 && dest->biHeight == 100);
    CPPUNIT_ASSERT(ctrl.free(dest));
    dest = NULL;

    // intersects
    CPPUNIT_ASSERT(ctrl.getDIBFromImage("black_1bit", Rect(-10, -10, 50, 50), NULL, &dest));
    CPPUNIT_ASSERT(dest);
    CPPUNIT_ASSERT(dest->biWidth == 40 && dest->biHeight == 40);
    CPPUNIT_ASSERT(ctrl.free(dest));
    dest = NULL;

    // bit mask
    BitMask mask(50, 50);
    mask.fillRect(Rect(10, 10, 30, 30), true);
    CPPUNIT_ASSERT(ctrl.getDIBFromImage("black_1bit", Rect(0, 0, 50, 50), &mask, &dest));
    CPPUNIT_ASSERT(dest);
    CPPUNIT_ASSERT(dest->biWidth == 50 && dest->biHeight == 50);
    CTIControl::writeDIBtoBMP("cimage_get_image_from_dib_1.bmp", dest);
    CPPUNIT_ASSERT(ctrl.free(dest));
    dest = NULL;

    // read mask
    ctrl.enableReadMask("black_1bit");
    ctrl.addRectToReadMask("black_1bit", Rect(10, 0, 30, 50));
    CPPUNIT_ASSERT(ctrl.getDIBFromImage("black_1bit", Rect(0, 0, 50, 50), NULL, &dest));
    CPPUNIT_ASSERT(dest);
    CPPUNIT_ASSERT(dest->biWidth == 50 && dest->biHeight == 50);
    CTIControl::writeDIBtoBMP("cimage_get_image_from_dib_2.bmp", dest);
    CPPUNIT_ASSERT(ctrl.free(dest));
    dest = NULL;

    delete[] handle;
}

