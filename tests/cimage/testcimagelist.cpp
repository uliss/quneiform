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

#include "testcimagelist.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCImageList);

#include "cimage/ctiimagelist.h"

void TestCImageList::testInit()
{
    cf::CTIImageList lst;

    CPPUNIT_ASSERT(!lst.image("none"));
}

void TestCImageList::testAddImage()
{
    cf::CTIImageList lst;
    CPPUNIT_ASSERT(!lst.addImage("", NULL, false));
    CPPUNIT_ASSERT(!lst.addImage("test", NULL, false));
    CPPUNIT_ASSERT(lst.addImage("test", (cf::BitmapPtr) 0xFFFF, true));
    CPPUNIT_ASSERT(lst.hasImage("test"));
}

void TestCImageList::testRemoveImage()
{
    cf::CTIImageList lst;
    CPPUNIT_ASSERT(!lst.hasImage("test"));
    CPPUNIT_ASSERT(!lst.removeImage("test"));
    CPPUNIT_ASSERT(lst.addImage("test", (cf::BitmapPtr) 0xFFFF, true));
    CPPUNIT_ASSERT(lst.hasImage("test"));
    CPPUNIT_ASSERT(lst.removeImage("test"));
    CPPUNIT_ASSERT(!lst.hasImage("test"));
}

void TestCImageList::testImage()
{
    cf::CTIImageList lst;
    CPPUNIT_ASSERT(!lst.image("test"));

    lst.addImage("test", (cf::BitmapPtr) 0xFFFF, true);
    CPPUNIT_ASSERT(lst.image("test"));
}
