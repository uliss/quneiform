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

#ifndef TESTHTMLEXPORTER_H_
#define TESTHTMLEXPORTER_H_

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "ced/cedpageptr.h"

namespace cf {
class HtmlExporter;
}

class TestHtmlExporter: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestHtmlExporter);
    CPPUNIT_TEST(testExport);
    CPPUNIT_TEST(testExportCharacter);
    CPPUNIT_TEST(testExportColumn);
    CPPUNIT_TEST(testExportLine);
    CPPUNIT_TEST(testExportParagraph);
    CPPUNIT_TEST(testExportPicture);

    CPPUNIT_TEST(testHyphens);

    CPPUNIT_TEST(testBold);
    CPPUNIT_TEST(testItalic);
    CPPUNIT_TEST(testMixed);
    CPPUNIT_TEST(testSub);
    CPPUNIT_TEST(testSuper);
    CPPUNIT_TEST(testUnderlined);
    CPPUNIT_TEST(testWriteAlternatives);
    CPPUNIT_TEST_SUITE_END();
    public:
        void setUp();
        void tearDown();

        void testExport();
        void testExportCharacter();
        void testExportColumn();
        void testExportLine();
        void testExportParagraph();
        void testExportPicture();

        void testHyphens();

        void testBold();
        void testItalic();
        void testMixed();
        void testSub();
        void testSuper();
        void testUnderlined();
        void testWriteAlternatives();
    private:
        cf::CEDPagePtr page_;
        cf::HtmlExporter * exp_;
        cf::CEDChar * c_;
        std::ostringstream buffer_;
};

#endif /* TESTHTMLEXPORTER_H_ */
