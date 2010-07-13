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
#include "teststyleexporter.h"
#include "export/styleexporter.cpp"
CPPUNIT_TEST_SUITE_REGISTRATION(TestStyleExporter);

using namespace CIF;

void TestStyleExporter::testHashChar() {
    FormatOptions opts;
    opts.useFontSize(false);
    StyleExporter e(NULL, opts);
    CEDChar c1, c2;

    CPPUNIT_ASSERT_EQUAL(e.hash(c1), e.hash(c2));
    c1.setColor(Color(100, 0, 0));
    CPPUNIT_ASSERT(e.hash(c1) != e.hash(c2));
    c2.setColor(c1.color());
    CPPUNIT_ASSERT_EQUAL(e.hash(c1), e.hash(c2));
    c1.setBackgroundColor(Color(0, 100, 0));
    CPPUNIT_ASSERT(e.hash(c1) != e.hash(c2));
    c2.setBackgroundColor(c1.backgroundColor());
    CPPUNIT_ASSERT_EQUAL(e.hash(c1), e.hash(c2));

    e.formatOptions().useFontSize(false);
    c1.setFontHeight(10);
    CPPUNIT_ASSERT_EQUAL(e.hash(c1), e.hash(c2));
    e.formatOptions().useFontSize(true);
    CPPUNIT_ASSERT(e.hash(c1) != e.hash(c2));
    c2.setFontHeight(c1.fontHeight());
    CPPUNIT_ASSERT_EQUAL(e.hash(c1), e.hash(c2));

    e.formatOptions().useItalic(false);
    c1.setFontStyle(FORMAT_FONT_ITALIC);
    CPPUNIT_ASSERT_EQUAL(e.hash(c1), e.hash(c2));
    e.formatOptions().useItalic(true);
    CPPUNIT_ASSERT(e.hash(c1) != e.hash(c2));
    c2.setFontStyle(FORMAT_FONT_ITALIC);
    CPPUNIT_ASSERT_EQUAL(e.hash(c1), e.hash(c2));

    e.formatOptions().useBold(false);
    c1.setFontStyle(FORMAT_FONT_BOLD);
    c2.setFontStyle(0);
    CPPUNIT_ASSERT_EQUAL(e.hash(c1), e.hash(c2));
    e.formatOptions().useBold(true);
    CPPUNIT_ASSERT(e.hash(c1) != e.hash(c2));
    c2.setFontStyle(FORMAT_FONT_BOLD);
    CPPUNIT_ASSERT_EQUAL(e.hash(c1), e.hash(c2));
}

void TestStyleExporter::testMakeStyle() {
    FormatOptions opts;
    StyleExporter e(NULL, opts);

    CEDChar chr;
    CPPUNIT_ASSERT_EQUAL(std::string("char_1"), e.makeStyle(chr));
    CPPUNIT_ASSERT_EQUAL(std::string("char_2"), e.makeStyle(chr));

    CEDParagraph par;
    CPPUNIT_ASSERT_EQUAL(std::string("par_1"), e.makeStyle(par));
    CPPUNIT_ASSERT_EQUAL(std::string("par_2"), e.makeStyle(par));
}
