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
#include "testqtimageloader.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestQtImageLoader);
#include <rdib/qtimageloader.h>
#include <cuneiform.h>
#include <common/debug.h>
#include <memory>
#include <sstream>
#include <fstream>

std::string trim(const std::string& str) {
    size_t pos = str.find_last_not_of(" \n");
    return pos == str.size() ? str : str.substr(0, pos + 1);
}

void ASSERT_BUFFER(const std::ostringstream& buf, const char * str) {
    CPPUNIT_ASSERT_EQUAL(trim(buf.str()), std::string(str));
}

using namespace cf;

void ASSERT_RECOGNIZE(const char * filename, const char * str) {
    QtImageLoader loader;
    ImagePtr img;
    std::ostringstream buf;
    CPPUNIT_ASSERT_NO_THROW(img = loader.load(std::string(LOADER_TEST_IMAGE_DIR) + filename));
    Puma::instance().open(img);
    Puma::instance().recognize();
    Puma::instance().save(buf, cf::FORMAT_DEBUG);
    Puma::instance().close();
    ASSERT_BUFFER(buf, str);
}

void TestQtImageLoader::testInit() {
    std::auto_ptr<QtImageLoader> loader(new QtImageLoader);
}

void TestQtImageLoader::testLoad() {
    std::string path = LOADER_TEST_IMAGE_DIR;
    std::auto_ptr<QtImageLoader> loader(new QtImageLoader);
    ImagePtr img;
    CPPUNIT_ASSERT_NO_THROW(img = loader->load(path + "test.xpm"));
    CPPUNIT_ASSERT(Size(1, 1) == img->size());
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.gif"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.tif"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.jpg"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.png"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.pbm"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.pgm"));
    CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.ppm"));
    //CPPUNIT_ASSERT_NO_THROW(loader->load(path + "test.bmp"));


    // throw
    CPPUNIT_ASSERT_THROW(loader->load("not-exists"), ImageLoader::Exception);
    std::ifstream is_empty;
    CPPUNIT_ASSERT_THROW(loader->load(is_empty), ImageLoader::Exception);
    std::ifstream is_bad;
    int tmp;
    is_bad >> tmp;
    CPPUNIT_ASSERT_THROW(loader->load(is_bad), ImageLoader::Exception);
}

void TestQtImageLoader::testLoadRecognize() {
    Config::instance().setDebug(false);
    std::ostream none(NULL);
    Debug().setOutput(none);

    // bmp
//    ASSERT_RECOGNIZE("english_32.bmp", "ENGLISH");
    ASSERT_RECOGNIZE("english_24.bmp", "ENGLISH");
    ASSERT_RECOGNIZE("english_16a.bmp", "ENGLISH");
//    ASSERT_RECOGNIZE("english_16b.bmp", "ENGLISH");
    ASSERT_RECOGNIZE("english_gray.bmp", "ENGLISH");
    ASSERT_RECOGNIZE("english_gray_rle.bmp", "ENGLISH");
    ASSERT_RECOGNIZE("english_indexed.bmp", "ENGLISH");
    ASSERT_RECOGNIZE("english_indexed_rle.bmp", "ENGLISH");
    ASSERT_RECOGNIZE("english_1.bmp", "ENGLISH");
}
