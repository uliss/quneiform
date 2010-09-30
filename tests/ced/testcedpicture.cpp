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
#include <fstream>
#include "testcedpicture.h"
#include "../test_common.h"
#include <ced/cedpicture.h>
#include <common/tostring.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDPicture);
using namespace cf;

void TestCEDPicture::testSerialize() {
#ifdef CF_SERIALIZE
    const char * fname = "serialize_cedpicture.txt";
    ImagePtr img_ptr(new Image(new uchar[100], 100, Image::AllocatorNew));
    img_ptr->setFileName("CED picture");
    CEDPicture p;
    p.setImage(img_ptr);

    writeToTextArchive(fname, p);

    CEDPicture new_p;
    readFromTextArchive(fname, new_p);
    CPPUNIT_ASSERT_EQUAL(p.image()->size(), new_p.image()->size());
    CPPUNIT_ASSERT_EQUAL(p.image()->fileName(), new_p.image()->fileName());
    CPPUNIT_ASSERT_EQUAL(p.align(), new_p.align());

#endif
}

void TestCEDPicture::testSerializeXml() {
#ifdef CF_SERIALIZE
    const char * fname = "serialize_cedpicture.xml";
    ImagePtr img_ptr(new Image(new uchar[100], 100, Image::AllocatorNew));
    img_ptr->setFileName("CED picture");
    CEDPicture p;
    p.setImage(img_ptr);

    writeToXmlArchive(fname, "cedpicture", p);

    CEDPicture new_p;
    readFromXmlArchive(fname, "cedpicture", new_p);
    CPPUNIT_ASSERT_EQUAL(p.image()->size(), new_p.image()->size());
    CPPUNIT_ASSERT_EQUAL(p.image()->fileName(), new_p.image()->fileName());
    CPPUNIT_ASSERT_EQUAL(p.align(), new_p.align());
#endif
}
