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

#include <boost/algorithm/string/trim.hpp>

#include "testpuma.h"
#include "common/tostring.h"
#include "common/imageurl.h"
#include "rdib/imageloaderfactory.h"
#include "cpage/cpage.h"
#include "ced/cedpage.h"
#include "export/summaryexporter.h"

#define private public
#include "puma/puma.h"
#include "puma/pumadef.h"

using namespace cf;
CPPUNIT_TEST_SUITE_REGISTRATION(TestPuma);

#ifndef TEST_IMG_PATH
#define TEST_IMG_PATH ""
#endif

void TestPuma::testInit() {
    Rect rect;

    Puma::instance().close();
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
    CPPUNIT_ASSERT(Puma::instance().input_dib_ == (BitmapPtr) ptr->data());
    CPPUNIT_ASSERT(Puma::instance().input_filename_ == "test");
    CPPUNIT_ASSERT(Puma::instance().cpage_ != NULL);
    Puma::instance().close();
}

void TestPuma::testLayoutTextBlocks()
{
    CPPUNIT_ASSERT(Puma::instance().textBlocks().empty());
    ImagePtr img = ImageLoaderFactory::instance().load(ImageURL(TEST_IMG_PATH "/english.png"));
    Puma::instance().open(img);
    Puma::instance().binarizeImage();
    Puma::instance().recognize();

    CPPUNIT_ASSERT(!Puma::instance().textBlocks().empty());
    CPPUNIT_ASSERT_EQUAL(Puma::instance().textBlocks().size(), size_t(1));
    Puma::instance().close();
}

void TestPuma::testAddTextBlock()
{
    CPPUNIT_ASSERT(Puma::instance().textBlocks().empty());
    ImagePtr img = ImageLoaderFactory::instance().load(ImageURL(TEST_IMG_PATH "/english.png"));
    Puma::instance().open(img);
    Puma::instance().binarizeImage();
    Puma::instance().prepare();
    Puma::instance().addTextBlock(Rect(10, 10, 100, 70));
    CPPUNIT_ASSERT(!Puma::instance().textBlocks().empty());
    CPPUNIT_ASSERT_EQUAL(Puma::instance().textBlocks().size(), size_t(1));
    CPPUNIT_ASSERT(CPAGE_GetCountBlock(Puma::instance().cpage_));
    Puma::instance().unsetUpdateFlag(FLG_UPDATE_CPAGE);
    Puma::instance().recognize();
    CPPUNIT_ASSERT_EQUAL(Puma::instance().textBlocks().size(), size_t(1));
    std::stringstream buf;
    Puma::instance().saveToText(buf);
    CPPUNIT_ASSERT_EQUAL(std::string("ENG"), boost::algorithm::trim_copy(buf.str()));
    Puma::instance().close();
}

void TestPuma::testAddImageBlock()
{
    CPPUNIT_ASSERT(Puma::instance().imageBlocks().empty());
    ImagePtr img = ImageLoaderFactory::instance().load(ImageURL(TEST_IMG_PATH "/english.png"));
    Puma::instance().open(img);
    Puma::instance().binarizeImage();
    Puma::instance().prepare();
    Puma::instance().addImageBlock(Rect(0, 0, 190, 40));
    CPPUNIT_ASSERT_EQUAL(Puma::instance().imageBlocks().size(), size_t(1));
    Puma::instance().recognize();
    Puma::instance().formatResult();
    CEDPagePtr ptr = Puma::instance().cedPage();
    SummaryExporter e(ptr, FormatOptions());
    e.exportTo(std::cerr);
    CPPUNIT_ASSERT_EQUAL(e.numPictures(), int(1));
    Puma::instance().close();
}
