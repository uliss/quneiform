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

#include <fstream>

#include "testcedmerge.h"
#include "common/log.h"
#include "export/cedpagemerge.h"
#include "export/htmlexporter.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"
#include "load/cuneiformtextloader.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDMerge);

#ifndef EXPORTER_TEST_IMAGE_DIR
#define EXPORTER_TEST_IMAGE_DIR "./"
#endif

#define NATIVE_SAMPLES_DIR EXPORTER_TEST_IMAGE_DIR

using namespace cf;

static CEDPagePtr load(const char * chr)
{
    std::string path = NATIVE_SAMPLES_DIR;
    path += chr;
    path += ".native";
    std::ifstream ifs(path.c_str());
    if(!ifs) {
        cfWarning() << "not found:" << path;
        return CEDPagePtr();
    }

    CuneiformTextLoader loader;
    return loader.load(ifs);
}

void TestCEDMerge::testAddPage()
{
    CEDPageMerge e;
    CPPUNIT_ASSERT(e.empty());

    e.add(CEDPagePtr());
    CPPUNIT_ASSERT(e.empty());

    CEDPagePtr p(new CEDPage);
    p->setImageName("test");
    p->addSection(new CEDSection);
    p->addSection(new CEDSection);

    e.add(p);
    CPPUNIT_ASSERT(!e.empty());
    CPPUNIT_ASSERT_EQUAL(e.get()->imageName(), p->imageName());

    // add twice
    e.add(p);
    CPPUNIT_ASSERT_EQUAL(size_t(4), e.get()->sectionCount());
}

void TestCEDMerge::testExport()
{
    CEDPageMerge merger;
    CPPUNIT_ASSERT(merger.empty());
    merger.add(load("italic"));
    CPPUNIT_ASSERT(!merger.empty());
    merger.add(load("bold"));

    const char * EXPORT_NAME = "ced_page_merge_export.html";
    cfInfo() << "exporting to:" << EXPORT_NAME;

    HtmlExporter html_exporter(merger.get());
    std::ofstream ofs(EXPORT_NAME);
    CPPUNIT_ASSERT(ofs);
    html_exporter.exportTo(ofs);
}
