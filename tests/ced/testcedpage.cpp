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
#include "testcedpage.h"
#include "../test_common.h"
#include "ced/cedchar.h"
#include "ced/cedpicture.h"
#include "ced/cedframe.h"
#include "ced/cedline.h"
#include "ced/cedparagraph.h"
#include "ced/cedcolumn.h"
#include "ced/cedsection.h"
#include "ced/cedpage.h"
#include "common/tostring.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDPage);

using namespace cf;

static CEDSection * firstSection(CEDPage& p) {
    return p.sectionAt(0);
}

static CEDColumn * firstColumn(CEDPage& p) {
    return firstSection(p)->columnAt(0);
}

static CEDParagraph * firstPar(CEDPage& p) {
    return dynamic_cast<CEDParagraph*>(firstColumn(p)->elementAt(0));
}

static CEDLine * firstLine(CEDPage& p) {
    return firstPar(p)->lineAt(0);
}

void TestCEDPage::testSerialize() {
#ifdef CF_SERIALIZE
    CEDPage p;
    p.setImageName("Test Image");
    Size sz(10, 20);
    p.setImageSize(sz);
    Size dpi(200, 300);
    p.setImageDpi(dpi);
    p.setLanguage(LANGUAGE_ITALIAN);
    Rect borders(Point(0, 0), 1000, 2000);
    p.setPageBorder(borders);
    p.addSection(new CEDSection);
    firstSection(p)->addColumn(new CEDColumn);
    firstColumn(p)->addElement(new CEDParagraph);
    firstPar(p)->addLine(new CEDLine);
    firstLine(p)->addElement(new CEDChar('t'));
    firstLine(p)->addElement(new CEDPicture);
    firstPar(p)->addElement(new CEDPicture);


    const char * fname = "serialize_cedpage.txt";

    writeToTextArchive(fname, p);

    CEDPage new_p;
    readFromTextArchive(fname, new_p);

    CPPUNIT_ASSERT_EQUAL(std::string("Test Image"), new_p.imageName());
    CPPUNIT_ASSERT_EQUAL(sz, new_p.imageSize());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_ITALIAN, new_p.language());
    CPPUNIT_ASSERT_EQUAL(dpi, new_p.imageDpi());
    CPPUNIT_ASSERT_EQUAL(borders, new_p.pageBorder());
#endif
}

void TestCEDPage::testSerializeXml() {
#ifdef CF_SERIALIZE
    CEDPage p;
    p.setImageName("Test Image");
    Size sz(10, 20);
    p.setImageSize(sz);
    Size dpi(200, 300);
    p.setImageDpi(dpi);
    p.setLanguage(LANGUAGE_ITALIAN);
    Rect borders(Point(0, 0), 1000, 2000);
    p.setPageBorder(borders);
    p.addSection(new CEDSection);
    firstSection(p)->addColumn(new CEDColumn);
    firstColumn(p)->addElement(new CEDParagraph);
    firstPar(p)->addLine(new CEDLine);
    firstLine(p)->addElement(new CEDChar('t'));
    firstLine(p)->addElement(new CEDPicture);
    firstPar(p)->addElement(new CEDPicture);

    const char * fname = "serialize_cedpage.xml";

    writeToXmlArchive(fname, "cedpage", p);

    CEDPage new_p;
    readFromXmlArchive(fname, "cedpage", new_p);

    CPPUNIT_ASSERT_EQUAL(std::string("Test Image"), new_p.imageName());
    CPPUNIT_ASSERT_EQUAL(sz, new_p.imageSize());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_ITALIAN, new_p.language());
    CPPUNIT_ASSERT_EQUAL(dpi, new_p.imageDpi());
    CPPUNIT_ASSERT_EQUAL(borders, new_p.pageBorder());
#endif
}

void TestCEDPage::testClone()
{
    CEDPage page;
    page.setImageSize(Size(10, 20));
    page.setImageDpi(Size(300, 400));
    page.setPageSize(Size(30, 50));
    page.setPageBorder(Rect(10, 20, 400, 400));
    page.setImageName("test page");
    page.setTurn(70);
    page.setPageNumber(2);
    page.setLanguage(LANGUAGE_TURKISH);


    CEDPage * page_copy = page.clone();
    CPPUNIT_ASSERT(page_copy);
    CPPUNIT_ASSERT_EQUAL(page.imageSize(), page_copy->imageSize());
    CPPUNIT_ASSERT_EQUAL(page.imageDpi(), page_copy->imageDpi());
    CPPUNIT_ASSERT_EQUAL(page.pageSize(), page_copy->pageSize());
    CPPUNIT_ASSERT_EQUAL(page.pageBorder(), page_copy->pageBorder());
    CPPUNIT_ASSERT_EQUAL(page.imageName(), page_copy->imageName());
    CPPUNIT_ASSERT_EQUAL(page.turn(), page_copy->turn());
    CPPUNIT_ASSERT_EQUAL(page.pageNumber(), page_copy->pageNumber());
    CPPUNIT_ASSERT_EQUAL(page.language(), page_copy->language());

    delete page_copy;
}
