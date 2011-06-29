/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include "testzipcpp.h"
#include "export/zipcpp.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestZipCpp);

#ifndef _WIN32
#include <unistd.h>
#endif

using namespace cf;

void TestZipCpp::testInit() {
    ZipCpp z;
    CPPUNIT_ASSERT(z.empty());

    z.addFile("test");
    CPPUNIT_ASSERT(!z.empty());
    CPPUNIT_ASSERT(z.hasFile("test"));
    CPPUNIT_ASSERT(z.fileCount() == 1);

    z.removeFile("test");
    CPPUNIT_ASSERT(z.fileCount() == 0);
    CPPUNIT_ASSERT(!z.hasFile("test"));
    CPPUNIT_ASSERT(z.empty());
}

void TestZipCpp::testAddFile() {
    ZipCpp z;

    z.addFile("file1");
    z.addFile("file1");
    CPPUNIT_ASSERT_EQUAL(z.fileCount(), (size_t) 1);
    CPPUNIT_ASSERT(z.hasFile("file1"));

    z.addFile("file2");
    CPPUNIT_ASSERT_EQUAL(z.fileCount(), (size_t) 2);
    CPPUNIT_ASSERT(z.hasFile("file2"));
}

void TestZipCpp::testRemoveFile() {
    ZipCpp z;
    z.addFile("file1");
    z.addFile("file2");

    CPPUNIT_ASSERT_EQUAL(z.fileCount(), (size_t) 2);
    z.removeFile("file1");
    CPPUNIT_ASSERT_EQUAL(z.fileCount(), (size_t) 1);
    CPPUNIT_ASSERT(z.hasFile("file2"));
    z.removeFile("file2");
    CPPUNIT_ASSERT(z.empty());
}

void TestZipCpp::testSave() {
    ZipCpp z;

    z.addFile("dir/file1");
    z.setContent("dir/file1", "SAMPLE");
    z.addFile("mime");
    z.setCompression("mime", ZipCpp::NONE);
    z.setContent("mime", "bla-bla-bla");
    z.addFile("file3");
    z.setSource("file3", __FILE__);
    CPPUNIT_ASSERT(z.save("test.zip"));
}
