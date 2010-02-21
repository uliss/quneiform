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
#include "testhtmlexporter.h"
#include <fstream>
#include <sstream>
CPPUNIT_TEST_SUITE_REGISTRATION(TestHtmlExporter);
#define private public

#include <rout/htmlexporter.h>
#include <ced/ced.h>
#include <ced/cedint.h>
using namespace CIF;

void TestHtmlExporter::testInit() {
    HtmlExporter * exp = new HtmlExporter(NULL);
    delete exp;
}

void TestHtmlExporter::testExport() {
    CEDPage * page = new CEDPage;
    HtmlExporter * exp = new HtmlExporter(page);

    // attempt to export to empty stream
    std::fstream os;
    CPPUNIT_ASSERT_THROW(exp->exportTo(os), Exporter::Exception);

    // export to stream buffer
    std::stringstream buf;
    exp->exportTo(buf);

    CPPUNIT_ASSERT(exp->current_font_style_ == 0);

    delete exp;
    delete page;
}

void TestHtmlExporter::testExportParagraph() {
    CEDPage * page = new CEDPage;
    HtmlExporter * exp = new HtmlExporter(page);
    CPPUNIT_ASSERT_EQUAL(true, exp->skipEmptyParagraphs());
    CEDParagraph * par = new CEDParagraph;
    par->alignment = TP_LEFT_ALLIGN;

    std::stringstream buf1;
    exp->os_ = &buf1;

    // empty paragraph
    exp->exportParagraph(par);
    CPPUNIT_ASSERT_EQUAL(std::string(""), buf1.str());
    CPPUNIT_ASSERT(exp->current_font_style_ == 0);

    // explicit exports
    // left align
    std::stringstream buf2;
    exp->os_ = &buf2;
    exp->setSkipEmptyParagraphs(false);
    exp->exportParagraph(par);
    CPPUNIT_ASSERT_EQUAL(std::string("<p></p>\n"), buf2.str());

    // right align
    std::stringstream buf3;
    exp->os_ = &buf3;
    par->alignment = TP_RIGHT_ALLIGN;
    exp->exportParagraph(par);
    CPPUNIT_ASSERT_EQUAL(std::string("<p align=\"right\"></p>\n"), buf3.str());

    // center align
    std::stringstream buf4;
    exp->os_ = &buf4;
    par->alignment = TP_CENTER;
    exp->exportParagraph(par);
    CPPUNIT_ASSERT_EQUAL(std::string("<p align=\"center\"></p>\n"), buf4.str());

    // justify align
    std::stringstream buf5;
    exp->os_ = &buf5;
    par->alignment = TP_RIGHT_ALLIGN | TP_LEFT_ALLIGN;
    exp->exportParagraph(par);
    CPPUNIT_ASSERT_EQUAL(std::string("<p align=\"justify\"></p>\n"), buf5.str());

    delete par;
    delete exp;
    delete page;
}

void TestHtmlExporter::testExportLine() {
    CEDPage * page = new CEDPage;
    HtmlExporter * exp = new HtmlExporter(page);
    CPPUNIT_ASSERT_EQUAL(true, exp->skipEmptyLines());
    CEDLine * line = new CEDLine;

    std::stringstream buf1;
    exp->os_ = &buf1;

    // empty line
    exp->exportLine(line);
    CPPUNIT_ASSERT_EQUAL(std::string(""), buf1.str());

    // explicit empty line export
    exp->setSkipEmptyLines(false);
    std::stringstream buf2;
    exp->os_ = &buf2;

    exp->exportLine(line);
    CPPUNIT_ASSERT_EQUAL(std::string(""), buf2.str());

    // line breaks

    std::stringstream buf3;
    exp->os_ = &buf3;

    // preserve line breaks
    FormatOptions opts;
    opts.setPreserveLineBreaks(true);
    exp->setFormatOptions(opts);
    exp->lines_left_ = 2;
    exp->exportLine(line);
    CPPUNIT_ASSERT_EQUAL(std::string("<br/>\n"), buf3.str());

    // line hard break
    std::stringstream buf4;
    exp->os_ = &buf4;
    FormatOptions opts2;
    opts.setPreserveLineBreaks(false);
    exp->setFormatOptions(opts2);
    line->hardBreak = TRUE;
    exp->lines_left_ = 2;
    exp->exportLine(line);
    CPPUNIT_ASSERT_EQUAL(std::string("<br/>\n"), buf4.str());

    delete exp;
    delete page;
    delete line;
}
