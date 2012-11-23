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

void TestCEDPicture::testClone()
{
    CEDPicture pict;
    pict.setAlign(ED_ALIGN_TOP);
    pict.setPictureNumber(23);
    ImagePtr img_ptr(new Image(new uchar[100], 100, Image::AllocatorNew));
    memset(img_ptr->data(), 0xFF, 100);
    img_ptr->setFileName("CED picture");
    pict.setImage(img_ptr);

    CEDPicture * pict_copy = pict.clone();
    CPPUNIT_ASSERT(pict_copy);
    CPPUNIT_ASSERT_EQUAL(pict.align(), pict_copy->align());
    CPPUNIT_ASSERT_EQUAL(pict.pictureNumber(), pict_copy->pictureNumber());
    ImagePtr img_data_copy = pict_copy->image();
    CPPUNIT_ASSERT_EQUAL(img_ptr->size(), img_data_copy->size());
    CPPUNIT_ASSERT_EQUAL(img_ptr->fileName(), img_data_copy->fileName());
    CPPUNIT_ASSERT_EQUAL(img_ptr->dataSize(), img_data_copy->dataSize());
    CPPUNIT_ASSERT(img_data_copy->data() != img_ptr->data());
    for(int i = 0; i < 100; i++) {
        CPPUNIT_ASSERT_EQUAL((int) img_data_copy->data()[i], (int) 0xFF);
    }
    delete pict_copy;
}
