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
#define protected public

#include <rout/htmlexporter.h>
#include <rout/rout_own.h>
#include <ced/ced.h>
#include <ced/cedint.h>
#include <ced/cedline.h>
#include <ced/cedpage.h>
using namespace CIF;

void TestHtmlExporter::setUp() {
    page_ = new CEDPage;
    exp_ = new HtmlExporter(page_);
}

void TestHtmlExporter::tearDown() {
    delete exp_;
    delete page_;
}

void TestHtmlExporter::testInit() {
}

void TestHtmlExporter::testExport() {
    // attempt to export to empty stream
    std::fstream os;
    CPPUNIT_ASSERT_THROW(exp_->exportTo(os), Exporter::Exception);

    // export to stream buffer
    std::stringstream buf;
    exp_->exportTo(buf);

    CPPUNIT_ASSERT(exp_->current_font_style_ == 0);
}

void TestHtmlExporter::testExportParagraph() {
    CPPUNIT_ASSERT_EQUAL(true, exp_->skipEmptyParagraphs());
    CEDParagraph * par = new CEDParagraph;
    par->alignment = TP_LEFT_ALLIGN;

    std::stringstream buf1;
    exp_->os_ = &buf1;

    // empty paragraph
    exp_->exportParagraph(par);
    CPPUNIT_ASSERT_EQUAL(std::string(""), buf1.str());
    CPPUNIT_ASSERT(exp_->current_font_style_ == 0);

    // explicit exports
    // left align
    std::stringstream buf2;
    exp_->os_ = &buf2;
    exp_->setSkipEmptyParagraphs(false);
    exp_->exportParagraph(par);
    CPPUNIT_ASSERT_EQUAL(std::string("<p></p>\n"), buf2.str());

    // right align
    std::stringstream buf3;
    exp_->os_ = &buf3;
    par->alignment = TP_RIGHT_ALLIGN;
    exp_->exportParagraph(par);
    CPPUNIT_ASSERT_EQUAL(std::string("<p align=\"right\"></p>\n"), buf3.str());

    // center align
    std::stringstream buf4;
    exp_->os_ = &buf4;
    par->alignment = TP_CENTER;
    exp_->exportParagraph(par);
    CPPUNIT_ASSERT_EQUAL(std::string("<p align=\"center\"></p>\n"), buf4.str());

    // justify align
    std::stringstream buf5;
    exp_->os_ = &buf5;
    par->alignment = TP_RIGHT_ALLIGN | TP_LEFT_ALLIGN;
    exp_->exportParagraph(par);
    CPPUNIT_ASSERT_EQUAL(std::string("<p align=\"justify\"></p>\n"), buf5.str());

    delete par;
}

void TestHtmlExporter::testExportLine() {
    CPPUNIT_ASSERT_EQUAL(true, exp_->skipEmptyLines());
    CEDLine * line = new CEDLine;

    std::stringstream buf1;
    exp_->os_ = &buf1;

    // empty line
    exp_->exportLine(line);
    CPPUNIT_ASSERT_EQUAL(std::string(""), buf1.str());

    // explicit empty line export
    exp_->setSkipEmptyLines(false);
    std::stringstream buf2;
    exp_->os_ = &buf2;

    exp_->exportLine(line);
    CPPUNIT_ASSERT_EQUAL(std::string(""), buf2.str());

    // line breaks

    std::stringstream buf3;
    exp_->os_ = &buf3;

    // preserve line breaks
    exp_->formatOptions().setPreserveLineBreaks(true);
    exp_->lines_left_ = 2;
    exp_->exportLine(line);
    CPPUNIT_ASSERT_EQUAL(std::string("<br/>\n"), buf3.str());

    // line hard break
    std::stringstream buf4;
    exp_->os_ = &buf4;
    exp_->formatOptions().setPreserveLineBreaks(false);
    line->setHardBreak(true);
    exp_->lines_left_ = 2;
    exp_->exportLine(line);
    CPPUNIT_ASSERT_EQUAL(std::string("<br/>\n"), buf4.str());
    delete line;
}

void TestHtmlExporter::testExportCharacter() {
    CEDChar * c = new CEDChar;
    c->alternatives = new LETTER;

    c->alternatives->alternative = '&';
    std::stringstream buf1;
    exp_->os_ = &buf1;
    exp_->writeCharacter(buf1, c);
    CPPUNIT_ASSERT_EQUAL(std::string("&amp;"), buf1.str());

    c->alternatives->alternative = '>';
    std::stringstream buf2;
    exp_->os_ = &buf2;
    exp_->writeCharacter(buf2, c);
    CPPUNIT_ASSERT_EQUAL(std::string("&gt;"), buf2.str());

    c->alternatives->alternative = '<';
    std::stringstream buf3;
    exp_->os_ = &buf3;
    exp_->writeCharacter(buf3, c);
    CPPUNIT_ASSERT_EQUAL(std::string("&lt;"), buf3.str());

    c->alternatives->alternative = '"';
    std::stringstream buf4;
    exp_->os_ = &buf4;
    exp_->writeCharacter(buf4, c);
    CPPUNIT_ASSERT_EQUAL(std::string("&quot;"), buf4.str());

    std::string chars("abcdefghijklmnopqstuvwxyz123456789!@#$%^*(-+");
    for (uint i = 0; i < chars.size(); i++) {
        c->alternatives->alternative = chars[i];
        std::stringstream buf;
        exp_->os_ = &buf;
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string(1, chars[i]), buf.str());
    }

    delete c->alternatives;
    delete c;
}

void TestHtmlExporter::testBold() {
    CEDChar * c = new CEDChar;
    c->alternatives = new LETTER;

    {
        exp_->formatOptions().useBold(true);
        c->alternatives->alternative = 'd';
        c->fontAttribs = FONT_BOLD;
        std::stringstream buf;
        exp_->os_ = &buf;
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string("<b>d"), buf.str());
        c->fontAttribs = 0;
        c->alternatives->alternative = 'e';
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string("<b>d</b>e"), buf.str());
    }

    {
        exp_->formatOptions().useBold(false);
        CPPUNIT_ASSERT(!exp_->formatOptions().isBoldUsed());
        c->alternatives->alternative = 'd';
        c->fontAttribs = FONT_BOLD;
        std::stringstream buf;
        exp_->os_ = &buf;
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string("d"), buf.str());
        c->fontAttribs = 0;
        c->alternatives->alternative = 'e';
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string("de"), buf.str());
    }

    exp_->formatOptions().useBold(true);

    delete c->alternatives;
    delete c;
}

void TestHtmlExporter::testItalic() {
    CEDChar * c = new CEDChar;
    c->alternatives = new LETTER;

    {
        exp_->formatOptions().useItalic(true);
        c->alternatives->alternative = '1';
        c->fontAttribs = FONT_ITALIC;
        std::stringstream buf;
        exp_->os_ = &buf;
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string("<i>1"), buf.str());
        c->fontAttribs = 0;
        c->alternatives->alternative = '2';
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string("<i>1</i>2"), buf.str());
    }

    {
        exp_->formatOptions().useItalic(false);
        CPPUNIT_ASSERT(!exp_->formatOptions().isItalicUsed());
        c->alternatives->alternative = '1';
        c->fontAttribs = FONT_ITALIC;
        std::stringstream buf;
        exp_->os_ = &buf;
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string("1"), buf.str());
        c->fontAttribs = 0;
        c->alternatives->alternative = '2';
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string("12"), buf.str());
    }

    exp_->formatOptions().useItalic(true);

    delete c->alternatives;
    delete c;
}

void TestHtmlExporter::testUnderlined() {
    CEDChar * c = new CEDChar;
    c->alternatives = new LETTER;

    c->alternatives->alternative = '1';
    c->fontAttribs = FONT_UNDERLINE;
    std::stringstream buf;
    exp_->os_ = &buf;
    exp_->writeCharacter(buf, c);
    CPPUNIT_ASSERT_EQUAL(std::string("<u>1"), buf.str());
    c->fontAttribs = 0;
    c->alternatives->alternative = '2';
    exp_->writeCharacter(buf, c);
    CPPUNIT_ASSERT_EQUAL(std::string("<u>1</u>2"), buf.str());

    delete c->alternatives;
    delete c;
}

void TestHtmlExporter::testSub() {
    CEDChar * c = new CEDChar;
    c->alternatives = new LETTER;

    exp_->formatOptions().useFontSize(true);
    c->alternatives->alternative = '1';
    c->fontAttribs = FONT_SUB;
    std::stringstream buf;
    exp_->os_ = &buf;
    exp_->writeCharacter(buf, c);
    CPPUNIT_ASSERT_EQUAL(std::string("<sub>1"), buf.str());
    c->fontAttribs = 0;
    c->alternatives->alternative++;
    exp_->writeCharacter(buf, c);
    CPPUNIT_ASSERT_EQUAL(std::string("<sub>1</sub>2"), buf.str());

    delete c->alternatives;
    delete c;
}

void TestHtmlExporter::testSuper() {
    CEDChar * c = new CEDChar;
    c->alternatives = new LETTER;

    exp_->formatOptions().useFontSize(true);
    c->alternatives->alternative = '1';
    c->fontAttribs = FONT_SUPER;
    std::stringstream buf;
    exp_->os_ = &buf;
    exp_->writeCharacter(buf, c);
    CPPUNIT_ASSERT_EQUAL(std::string("<sup>1"), buf.str());
    c->fontAttribs = 0;
    c->alternatives->alternative++;
    exp_->writeCharacter(buf, c);
    CPPUNIT_ASSERT_EQUAL(std::string("<sup>1</sup>2"), buf.str());

    delete c->alternatives;
    delete c;
}

void TestHtmlExporter::testMixed() {
    CEDChar * c = new CEDChar;
    c->alternatives = new LETTER;

    exp_->formatOptions().useFontSize(true);
    c->alternatives->alternative = '1';
    c->fontAttribs = FONT_UNDERLINE | FONT_ITALIC | FONT_SUB;
    std::stringstream buf;
    exp_->os_ = &buf;
    exp_->writeCharacter(buf, c);
    CPPUNIT_ASSERT_EQUAL(std::string("<sub><u><i>1"), buf.str());
    c->fontAttribs = 0;
    c->alternatives->alternative = '2';
    exp_->writeCharacter(buf, c);
    CPPUNIT_ASSERT_EQUAL(std::string("<sub><u><i>1</i></u></sub>2"), buf.str());

    {
        c->alternatives->alternative = '1';
        c->fontAttribs = FONT_UNDERLINE | FONT_ITALIC | FONT_SUB | FONT_SUPER;
        std::stringstream buf;
        exp_->os_ = &buf;
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string("<sup><sub><u><i>1"), buf.str());
        c->fontAttribs = 0;
        c->alternatives->alternative = '2';
        exp_->writeCharacter(buf, c);
        c->alternatives->alternative++;
        c->fontAttribs = FONT_UNDERLINE;
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string("<sup><sub><u><i>1</i></u></sub></sup>2<u>3"), buf.str());
    }

    {
        c->alternatives->alternative = '1';
        c->fontAttribs = FONT_BOLD | FONT_ITALIC;
        std::stringstream buf;
        exp_->font_styles_.clear();
        exp_->os_ = &buf;
        exp_->writeCharacter(buf, c);
        CPPUNIT_ASSERT_EQUAL(std::string("<i><b>1"), buf.str());
        c->fontAttribs = FONT_SUB | FONT_ITALIC;
        c->alternatives->alternative++;
        exp_->writeCharacter(buf, c);
        c->alternatives->alternative++;
        c->fontAttribs = FONT_UNDERLINE;
        exp_->writeCharacter(buf, c);
        //CPPUNIT_ASSERT_EQUAL(std::string("<i><b>1</b><sub>2</sub></i><u>3"), buf.str());
    }

    delete c->alternatives;
    delete c;
}

void TestHtmlExporter::testFontStyleClose() {
    exp_->font_styles_.clear();
    exp_->current_font_style_ = 0;

    {
        std::stringstream buf;
        exp_->writeFontStyleClose(buf, 0, FONT_ITALIC);
        CPPUNIT_ASSERT_EQUAL(std::string(""), buf.str());
    }

    {
        exp_->font_styles_.push_back(FONT_ITALIC);
        std::stringstream buf;
        exp_->current_font_style_ = FONT_ITALIC;
        exp_->writeFontStyleClose(buf, 0, FONT_ITALIC);
        CPPUNIT_ASSERT_EQUAL(std::string("</i>"), buf.str());
        CPPUNIT_ASSERT(exp_->font_styles_.size() == 0);
    }

    {
        exp_->font_styles_.push_back(FONT_BOLD);
        exp_->font_styles_.push_back(FONT_ITALIC);
        std::stringstream buf;
        exp_->current_font_style_ = FONT_ITALIC;
        exp_->writeFontStyleClose(buf, 0, FONT_ITALIC);
        CPPUNIT_ASSERT_EQUAL(std::string("</i>"), buf.str());
        CPPUNIT_ASSERT(exp_->font_styles_.size() == 1);
        CPPUNIT_ASSERT(exp_->font_styles_[0] = FONT_BOLD);
    }

    {
        exp_->font_styles_.clear();
        exp_->font_styles_.push_back(FONT_ITALIC);
        exp_->font_styles_.push_back(FONT_BOLD);
        std::stringstream buf;
        exp_->current_font_style_ = FONT_ITALIC;
        exp_->writeFontStyleClose(buf, 0, FONT_ITALIC);
        CPPUNIT_ASSERT_EQUAL(std::string("</b></i><b>"), buf.str());
        CPPUNIT_ASSERT(exp_->font_styles_.size() == 1);
        CPPUNIT_ASSERT(exp_->font_styles_[0] = FONT_BOLD);
    }
}

void TestHtmlExporter::testExportPicture() {
    CEDChar * pict = new CEDChar;
    pict->fontNum = ED_PICT_BASE;

    std::stringstream buf;
    exp_->os_ = &buf;
    // no page name
    //CPPUNIT_ASSERT_THROW(exp_->exportPicture(pict), Exporter::Exception);

    //exp_->output_filename_ = "output.html";
    //exp_->exportPicture(pict);

    //CPPUNIT_ASSERT_EQUAL(std::string("<img alt=\"\" src=\"output_files/image_0.bmp\"/>"), buf.str());
}

