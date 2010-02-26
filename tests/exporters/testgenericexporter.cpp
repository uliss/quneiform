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
#include <ced/cedint.h>
#include "cfcompat.h"
using namespace CIF;

void TestGenericExporter::testInit() {
    FormatOptions opt;
    GenericExporter * p = new GenericExporter(NULL, opt);

    CPPUNIT_ASSERT(p->page() == NULL);
    CPPUNIT_ASSERT(p->os_ == NULL);
    delete p;
}

void TestGenericExporter::testDoExport() {
    FormatOptions opt;
    CEDPage * page = new CEDPage;
    GenericExporter * p = new GenericExporter(page, opt);
    p->no_pictures_ = false;

    // test invalid stream
    std::ofstream f;
    f << 1;
    CPPUNIT_ASSERT_THROW(p->doExport(f), Exporter::Exception);

    // test standard output
    p->doExport(std::cout);
    CPPUNIT_ASSERT_EQUAL(p->no_pictures_, true);
    CPPUNIT_ASSERT(p->os_ == &std::cout);
    delete p;
    delete page;
}

void TestGenericExporter::testExportPage() {
    FormatOptions opt;
    CEDPage * page = new CEDPage;
    GenericExporter * p = new GenericExporter(page, opt);

    p->os_ = &std::cout;
    p->exportPage();

    delete p;
    delete page;
}

void TestGenericExporter::testCreatePicturesFolder() {
    FormatOptions opt;
    GenericExporter * p = new GenericExporter(NULL, opt);

    CPPUNIT_ASSERT_THROW(p->createPicturesFolder(), Exporter::Exception);

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
