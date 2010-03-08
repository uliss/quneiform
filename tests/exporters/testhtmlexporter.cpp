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

#include <export/htmlexporter.h>
#include <export/rout_own.h>
#include <ced/ced.h>
#include <ced/cedint.h>
#include <ced/cedline.h>
#include <ced/cedpage.h>
using namespace CIF;
using namespace std;

inline void clearBuffer(HtmlExporter * exp) {
    exp->lineBuffer().rdbuf()->str("");
}

inline string buffer(HtmlExporter * exp) {
    return exp->lineBuffer().str();
}

#define CHECK_BUFFER(str)  { \
    CPPUNIT_ASSERT_EQUAL(string(str), buffer(exp_));\
}

#define CHECK_BUFFER_CLEAR(str)  { \
    CPPUNIT_ASSERT_EQUAL(string(str), buffer(exp_));\
    clearBuffer(exp_);\
}

#define ASSERT_CHAR_WRITE(src, dest) {\
    c_->alternatives->alternative = src;\
    exp_->exportChar(c_);\
    CHECK_BUFFER(dest);\
}

#define ASSERT_CHAR_WRITE_CLEAR(src, dest) {\
    c_->alternatives->alternative = src;\
    exp_->exportChar(c_);\
    CHECK_BUFFER_CLEAR(dest);\
}

void TestHtmlExporter::setUp() {
    page_ = new CEDPage;
    exp_ = new HtmlExporter(page_);
    c_ = new CEDChar;
    c_->alternatives = new LETTER;
}

void TestHtmlExporter::tearDown() {
    delete exp_;
    delete page_;
    delete c_->alternatives;
    delete c_;
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
    exp_->lines_left_in_paragraph_ = 2;
    exp_->exportLine(line);
    CPPUNIT_ASSERT_EQUAL(std::string("<br/>\n"), buf3.str());

    // line hard break
    std::stringstream buf4;
    exp_->os_ = &buf4;
    exp_->formatOptions().setPreserveLineBreaks(false);
    line->setHardBreak(true);
    exp_->lines_left_in_paragraph_ = 2;
    exp_->exportLine(line);
    CPPUNIT_ASSERT_EQUAL(std::string("<br/>\n"), buf4.str());
    delete line;
}

void TestHtmlExporter::testExportCharacter() {
    // check special chars escaping
    ASSERT_CHAR_WRITE_CLEAR('&', "&amp;");
    ASSERT_CHAR_WRITE_CLEAR('>', "&gt;");
    ASSERT_CHAR_WRITE_CLEAR('<', "&lt;");
    ASSERT_CHAR_WRITE_CLEAR('"', "&quot;");
    ASSERT_CHAR_WRITE_CLEAR('\'', "&apos;");

    string chars("abcdefghijklmnopqstuvwxyz123456789!@#$%^*(-+");
    for (uint i = 0; i < chars.size(); i++) {
        c_->alternatives->alternative = chars[i];
        exp_->writeCharacter(cerr, c_);
        CPPUNIT_ASSERT_EQUAL(string(1, chars[i]), buffer(exp_));
        clearBuffer(exp_);
    }
}

void TestHtmlExporter::testBold() {
    exp_->formatOptions().useBold(true);

    c_->fontAttribs = FONT_BOLD;
    ASSERT_CHAR_WRITE('d', "<b>d");
    c_->fontAttribs = 0;
    ASSERT_CHAR_WRITE_CLEAR('e', "<b>d</b>e");

    exp_->formatOptions().useBold(false);
    CPPUNIT_ASSERT(!exp_->formatOptions().isBoldUsed());

    c_->fontAttribs = FONT_BOLD;
    ASSERT_CHAR_WRITE('d', "d");
    c_->fontAttribs = 0;
    ASSERT_CHAR_WRITE('e', "de");

    exp_->formatOptions().useBold(true);
}

void TestHtmlExporter::testItalic() {
    exp_->formatOptions().useItalic(true);

    c_->fontAttribs = FONT_ITALIC;
    ASSERT_CHAR_WRITE('1', "<i>1");
    c_->fontAttribs = 0;
    ASSERT_CHAR_WRITE_CLEAR('2', "<i>1</i>2");

    exp_->formatOptions().useItalic(false);
    CPPUNIT_ASSERT(!exp_->formatOptions().isItalicUsed());

    c_->fontAttribs = FONT_ITALIC;
    ASSERT_CHAR_WRITE('1', "1");
    c_->fontAttribs = 0;
    ASSERT_CHAR_WRITE('2', "12");

    exp_->formatOptions().useItalic(true);
}

void TestHtmlExporter::testUnderlined() {
    c_->fontAttribs = FONT_UNDERLINE;
    ASSERT_CHAR_WRITE('1', "<u>1");
    c_->fontAttribs = 0;
    ASSERT_CHAR_WRITE('2', "<u>1</u>2");
}

void TestHtmlExporter::testSub() {
    exp_->formatOptions().useFontSize(true);
    c_->fontAttribs = FONT_SUB;
    ASSERT_CHAR_WRITE('1', "<sub>1");
    c_->fontAttribs = 0;
    ASSERT_CHAR_WRITE('2', "<sub>1</sub>2");
}

void TestHtmlExporter::testSuper() {
    exp_->formatOptions().useFontSize(true);
    c_->fontAttribs = FONT_SUPER;
    ASSERT_CHAR_WRITE('1', "<sup>1");
    c_->fontAttribs = 0;
    ASSERT_CHAR_WRITE('2', "<sup>1</sup>2");
}

void TestHtmlExporter::testMixed() {
    exp_->formatOptions().useFontSize(true);
    c_->fontAttribs = FONT_UNDERLINE | FONT_ITALIC | FONT_SUB;
    ASSERT_CHAR_WRITE('1', "<i><u><sub>1");
    c_->fontAttribs = 0;
    ASSERT_CHAR_WRITE_CLEAR('2', "<i><u><sub>1</sub></u></i>2");

    c_->fontAttribs = FONT_UNDERLINE | FONT_ITALIC | FONT_SUB | FONT_SUPER;
    ASSERT_CHAR_WRITE('1', "<i><u><sub><sup>1");
    c_->fontAttribs = 0;
    ASSERT_CHAR_WRITE('2', "<i><u><sub><sup>1</sup></sub></u></i>2");
    c_->fontAttribs = FONT_UNDERLINE;
    ASSERT_CHAR_WRITE_CLEAR('3', "<i><u><sub><sup>1</sup></sub></u></i>2<u>3");

    {
        exp_->previous_style_ = 0;
        c_->fontAttribs = FONT_BOLD | FONT_ITALIC;
        ASSERT_CHAR_WRITE('1', "<b><i>1");
        c_->fontAttribs = FONT_SUB | FONT_ITALIC;

        // TODO uliss
        //ASSERT_CHAR_WRITE('2', "<b><i>1</i></b><i><sub>2");
        c_->fontAttribs = FONT_UNDERLINE;
        //ASSERT_CHAR_WRITE('3', "<i><b>1</b><sub>2</sub></i><u>3");
    }

}

void TestHtmlExporter::testFontStyleClose() {
    std::stringstream buf;
    exp_->writeFontStyleEnd(buf, FONT_ITALIC);
    CHECK_BUFFER("</i>");
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

