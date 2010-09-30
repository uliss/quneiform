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
#include "testpuma.h"
#define private public
#include <puma/puma.h>
#include <common/tostring.h>
using namespace cf;
CPPUNIT_TEST_SUITE_REGISTRATION(TestPuma);

void TestPuma::testInit() {
    Puma::instance().pageTemplate();
    Rect rect;
    //    CPPUNIT_ASSERT_EQUAL(rect, Puma::instance().pageTemplate());
    CPPUNIT_ASSERT(Puma::instance().input_dib_ == NULL);
    CPPUNIT_ASSERT(Puma::instance().input_filename_.empty());
    CPPUNIT_ASSERT(Puma::instance().cpage_ == NULL);
}

void TestPuma::testOpen() {
    ImagePtr null_ptr;
    CPPUNIT_ASSERT_THROW(Puma::instance().open(null_ptr), PumaException);

    const int sz = 20;
    uchar * data = new uchar[sz];
    ImagePtr ptr(new Image(data, sz, Image::AllocatorNew));
    ptr->setFileName("test");

    Puma::instance().open(ptr);
    CPPUNIT_ASSERT(Puma::instance().input_dib_ == ptr->data());
    CPPUNIT_ASSERT(Puma::instance().input_filename_ == "test");
    CPPUNIT_ASSERT(Puma::instance().cpage_ != NULL);
}
