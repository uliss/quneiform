/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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
#include "testbmp.h"
#include <rdib/bmpimageloader.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestBmpLoader);
using namespace CIF;

void TestBmpLoader::testInit()
{
    std::auto_ptr<ImageLoader> loader(new BmpImageLoader);
    ImagePtr image = loader->load(LOADER_TEST_IMAGE_DIR + std::string("test.bmp"));
    CPPUNIT_ASSERT_THROW(loader->load(LOADER_TEST_IMAGE_DIR + std::string("test_not_exists.bmp")), ImageLoader::Exception);
}

void TestBmpLoader::testMagick()
{
    std::auto_ptr<ImageLoader> loader(new BmpImageLoader);
    //    test empty
    std::stringstream is;
    CPPUNIT_ASSERT_THROW(loader->load(is), CIF::ImageLoader::Exception);
    is.clear();
    //    test wrong magick
    is << "GB";
    CPPUNIT_ASSERT_THROW(loader->load(is), CIF::ImageLoader::Exception);
}
