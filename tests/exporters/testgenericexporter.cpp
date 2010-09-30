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
#include "testgenericexporter.h"
#include <fstream>
CPPUNIT_TEST_SUITE_REGISTRATION(TestGenericExporter);
#define private public
#define protected public
#include <export/genericexporter.h>
#include <ced/cedpicture.h>
#include <ced/cedpage.h>
#include "cfcompat.h"
using namespace cf;

void TestGenericExporter::testInit() {
    FormatOptions opt;
    GenericExporter * p = new GenericExporter(NULL, opt);

    CPPUNIT_ASSERT(p->page_ == NULL);
    CPPUNIT_ASSERT(p->os_ == NULL);
    delete p;
}

void TestGenericExporter::testDoExport() {
    FormatOptions opt;
    CEDPage * page = new CEDPage;
    GenericExporter * p = new GenericExporter(page, opt);
    p->skip_pictures_ = false;

    // test invalid stream
    std::ofstream f;
    f << 1;
    CPPUNIT_ASSERT_THROW(p->doExport(f), Exporter::Exception);

    // test standard output
    p->doExport(std::cout);
    CPPUNIT_ASSERT_EQUAL(p->skip_pictures_, false);
    CPPUNIT_ASSERT(p->os_ == &std::cout);
    delete p;
    delete page;
}

void TestGenericExporter::testExportPage() {
    FormatOptions opt;
    CEDPage page;
    GenericExporter * p = new GenericExporter(&page, opt);

    p->os_ = &std::cout;
    p->exportPage(page);

    delete p;
}

void TestGenericExporter::testCreatePicturesFolder() {
    FormatOptions opt;
    GenericExporter * p = new GenericExporter(NULL, opt);

    p->setOutputFilename("test_page_1.html");
    p->createPicturesFolder();
    CPPUNIT_ASSERT_EQUAL(0, _access("test_page_1_files", 0));
    RemoveDirectory("test_page_1_files");

    p->setOutputFilename("./test_page_2.html");
    p->createPicturesFolder();
    CPPUNIT_ASSERT_EQUAL(0, _access("test_page_2_files", 0));
    RemoveDirectory("test_page_2_files");

    // assumed that tests not runs under root))))))))
    p->setOutputFilename("/test");
    CPPUNIT_ASSERT_THROW(p->createPicturesFolder(), Exporter::Exception);

    delete p;
}

void TestGenericExporter::testMakePictureName() {
    FormatOptions opt;
    GenericExporter * p = new GenericExporter(NULL, opt);
    CEDPicture pict;
    pict.setPictureNumber(1);
    CPPUNIT_ASSERT_EQUAL(std::string("image_1."), p->makePictureName(pict));
    delete p;
}

void TestGenericExporter::testMakeOutputPictureDir() {
    FormatOptions opt;
    GenericExporter * p = new GenericExporter(NULL, opt);

    // default dir
    CPPUNIT_ASSERT_EQUAL(std::string("cuneiform-out_files"), p->makeOutputPictureDir());
    p->setOutputFilename("./test.html");
    CPPUNIT_ASSERT_EQUAL(std::string("./test_files"), p->makeOutputPictureDir());
    p->setOutputFilename("test.html");
    CPPUNIT_ASSERT_EQUAL(std::string("test_files"), p->makeOutputPictureDir());
    p->setOutputFilename("/../..//test.html");
    CPPUNIT_ASSERT_EQUAL(std::string("/../..//test_files"), p->makeOutputPictureDir());
    delete p;
}

void TestGenericExporter::testMakePicturePath() {
    FormatOptions opt;
    GenericExporter * p = new GenericExporter(NULL, opt);
    CEDPicture pict;
    pict.setPictureNumber(1);

    // default dir
    CPPUNIT_ASSERT_EQUAL(std::string("cuneiform-out_files/image_1."), p->makePicturePath(pict));
    p->setOutputFilename("./test.html");
    CPPUNIT_ASSERT_EQUAL(std::string("./test_files/image_1."), p->makePicturePath(pict));

    pict.setPictureNumber(2);
    p->setOutputFilename("test.html");
    CPPUNIT_ASSERT_EQUAL(std::string("test_files/image_2."), p->makePicturePath(pict));

    pict.setPictureNumber(3);
    p->setOutputFilename("/../..//test.html");
    CPPUNIT_ASSERT_EQUAL(std::string("/../..//test_files/image_3."), p->makePicturePath(pict));
    delete p;
}

void TestGenericExporter::testMakePicturePathRelative() {
    FormatOptions opt;
    GenericExporter * p = new GenericExporter(NULL, opt);
    CEDPicture pict;
    pict.setPictureNumber(1);

    // default dir
    CPPUNIT_ASSERT_EQUAL(std::string("cuneiform-out_files/image_1."), p->makePicturePathRelative(pict));
    p->setOutputFilename("./test.html");
    CPPUNIT_ASSERT_EQUAL(std::string("test_files/image_1."), p->makePicturePathRelative(pict));

    pict.setPictureNumber(2);
    p->setOutputFilename("test.html");
    CPPUNIT_ASSERT_EQUAL(std::string("test_files/image_2."), p->makePicturePathRelative(pict));

    pict.setPictureNumber(3);
    p->setOutputFilename("/../..//test.html");
    CPPUNIT_ASSERT_EQUAL(std::string("test_files/image_3."), p->makePicturePathRelative(pict));
    delete p;
}
