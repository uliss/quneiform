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
#include "testcssexporter.h"
#include "export/cssexporter.h"
#include "ced/cedchar.h"
#include "ced/cedparagraph.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCssExporter);

#define CSS_INDENT "    "

using namespace CIF;

void TestCssExporter::testMakeCssStyleChar() {
    FormatOptions opts;
    CssExporter e(NULL, opts);

    CEDChar chr;
    CPPUNIT_ASSERT_EQUAL(std::string(""), e.makeCssStyle(chr));

    chr.setColor(Color(16, 0, 0));
    CPPUNIT_ASSERT_EQUAL(std::string(CSS_INDENT "color: #100000;\n"), e.makeCssStyle(chr));
    chr.setColor(Color::null());
    chr.setBackgroundColor(Color(0, 0, 16));
    CPPUNIT_ASSERT_EQUAL(std::string(CSS_INDENT "background-color: #000010;\n"), e.makeCssStyle(chr));
    chr.setBackgroundColor(Color::null());

    e.formatOptions().useFontSize(true);
    chr.setFontHeight(10);
    CPPUNIT_ASSERT_EQUAL(std::string(CSS_INDENT "font-size: 10px;\n"), e.makeCssStyle(chr));
    e.formatOptions().useFontSize(false);
    CPPUNIT_ASSERT_EQUAL(std::string(""), e.makeCssStyle(chr));
}

void TestCssExporter::testMakeCssStyleParagraph() {
    FormatOptions opts;
    CssExporter e(NULL, opts);

    CEDParagraph par;
    CPPUNIT_ASSERT_EQUAL(std::string(""), e.makeCssStyle(par));
    par.setColor(Color(16, 0, 0));
    CPPUNIT_ASSERT_EQUAL(std::string(CSS_INDENT "color: #100000;\n"), e.makeCssStyle(par));
    par.setColor(Color::null());
    par.setBackgroundColor(Color(0, 0, 16));
    CPPUNIT_ASSERT_EQUAL(std::string(CSS_INDENT "background-color: #000010;\n"), e.makeCssStyle(par));
    par.setBackgroundColor(Color::null());

    par.setIndent(10);
    CPPUNIT_ASSERT_EQUAL(std::string(CSS_INDENT "text-indent: 10px;\n"), e.makeCssStyle(par));
    par.setIndent(-10);
    CPPUNIT_ASSERT_EQUAL(std::string(CSS_INDENT "text-indent: -10px;\n"), e.makeCssStyle(par));
    par.setIndent(0);

    par.setAlign(ALIGN_JUSTIFY);
    CPPUNIT_ASSERT_EQUAL(std::string(CSS_INDENT "text-align: justify;\n"), e.makeCssStyle(par));
    par.setAlign(ALIGN_CENTER);
    CPPUNIT_ASSERT_EQUAL(std::string(CSS_INDENT "text-align: center;\n"), e.makeCssStyle(par));
    par.setAlign(ALIGN_RIGHT);
    CPPUNIT_ASSERT_EQUAL(std::string(CSS_INDENT "text-align: right;\n"), e.makeCssStyle(par));
    par.setAlign(ALIGN_NONE);
    CPPUNIT_ASSERT_EQUAL(std::string(""), e.makeCssStyle(par));
    par.setAlign(ALIGN_LEFT);
    CPPUNIT_ASSERT_EQUAL(std::string(""), e.makeCssStyle(par));
}

void TestCssExporter::testMakeStyle() {
}
