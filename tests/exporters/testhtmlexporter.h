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

#include <cppunit/extensions/HelperMacros.h>

namespace CIF {
class CEDPage;
class CEDChar;
class HtmlExporter;
}

class TestHtmlExporter: public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestHtmlExporter);
    CPPUNIT_TEST(testInit);
    CPPUNIT_TEST(testExport);
    CPPUNIT_TEST(testExportParagraph);
    CPPUNIT_TEST(testExportLine);
    CPPUNIT_TEST(testExportCharacter);
    CPPUNIT_TEST(testBold);
    CPPUNIT_TEST(testItalic);
    CPPUNIT_TEST(testUnderlined);
    CPPUNIT_TEST(testSub);
    CPPUNIT_TEST(testSuper);
    CPPUNIT_TEST(testMixed);
    CPPUNIT_TEST(testFontStyleClose);
    CPPUNIT_TEST(testExportPicture);
    CPPUNIT_TEST_SUITE_END();
    public:
        void setUp();
        void tearDown();

        void testInit();
        void testExport();
        void testExportParagraph();
        void testExportLine();
        void testExportCharacter();
        void testBold();
        void testItalic();
        void testUnderlined();
        void testSub();
        void testSuper();
        void testMixed();
        void testFontStyleClose();
        void testExportPicture();
    private:
        CIF::CEDPage * page_;
        CIF::HtmlExporter * exp_;
        CIF::CEDChar * c_;
        CIF::CEDChar * prev_c_;
};

#endif /* TESTHTMLEXPORTER_H_ */
