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
#include "rdib/ctdib.h"
#include "cimage/cticontrol.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCTDIB);

static void * t_alloc(uint32_t size) { return malloc(size); }
static void t_free(void * mem) { free(mem); }
static void * t_lock(void * mem) { return mem; }
static void t_unlock(void *) {}

void TestCTDIB::testInit()
{
    CTDIB image;
    CPPUNIT_ASSERT(image.SetExternals(t_alloc, t_free, t_lock, t_unlock));

    CPPUNIT_ASSERT(image.CreateDIBBegin(10, 20, 24));
    CPPUNIT_ASSERT(image.CreateDIBEnd());

    cf::CTIControl::writeDIBtoBMP("test_rdib.bmp", &image);

    for(int i = 0; i < image.GetImageWidth(); i++) {
        uchar * p = (uchar*) image.GetPtrToPixel(i, 0);
        p[0] = 0xff;
        p[1] = 0xff;
        p[2] = 0x00;
    }

    cf::CTIControl::writeDIBtoBMP("test_rdib_draw.bmp", &image);
}
