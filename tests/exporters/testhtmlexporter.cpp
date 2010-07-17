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
#include <ced/cedcolumn.h>
#include <ced/cedline.h>
#include <ced/cedpage.h>
#include <ced/cedchar.h>
#include <ced/cedpicture.h>
#include <ced/cedparagraph.h>
#include <rdib/imageloaderfactory.h>
using namespace CIF;
using namespace std;

inline void clearBuffer(HtmlExporter * exp) {
    exp->lineBuffer().rdbuf()->str("");
}

inline string buffer(HtmlExporter * exp) {
    return exp->lineBuffer().str();
}

inline CEDChar * makeChar(char l, int style = 0) {
    CEDChar * c = new CEDChar;
    c->addAlternative(Letter(l));
    c->setFontStyle(style);
    return c;
}

inline CEDChar * lineAddChar(CEDLine * l, char letter, int style = 0) {
    CEDChar * c = makeChar(letter, style);
    l->addElement(c);
    return c;
}

inline CEDChar * parAddChar(CEDParagraph& p, char letter, int style = 0) {
    if (!p.lineCount())
        p.addLine(new CEDLine);

    return lineAddChar(p.lineAt(0), letter, style);
}

#define CHECK_BUFFER(str)  { \
    CPPUNIT_ASSERT_EQUAL(string(str), buffer(exp_));\
}

#define CHECK_BUFFER_CLEAR(str)  { \
    CPPUNIT_ASSERT_EQUAL(string(str), buffer(exp_));\
    clearBuffer(exp_);\
}

#define ASSERT_CHAR_WRITE(src, dest) {\
    c_->setAlternative(Letter(src), 0);\
    exp_->exportChar(*c_);\
    CHECK_BUFFER(dest);\
}

#define ASSERT_CHAR_WRITE_CLEAR(src, dest) {\
	c_->setAlternative(Letter(src), 0);\
    exp_->exportChar(*c_);\
    CHECK_BUFFER_CLEAR(dest);\
}

#define CHECK_PAR(s, par) {\
    clearBuffer(exp_);\
    buffer_.str("");\
    exp_->exportParagraph(par);\
    CPPUNIT_ASSERT_EQUAL(std::string(s), buffer_.str());\
}

#define CHECK_COL(s, col) {\
    clearBuffer(exp_);\
    buffer_.str("");\
    exp_->exportColumn(col);\
    CPPUNIT_ASSERT_EQUAL(std::string(s), buffer_.str());\
}

void TestHtmlExporter::setUp() {
    page_ = new CEDPage;
    exp_ = new HtmlExporter(page_);
    exp_->os_ = &buffer_;
    c_ = new CEDChar;
    c_->addAlternative(Letter());
}

void TestHtmlExporter::tearDown() {
    delete exp_;
    delete page_;
    delete c_;
    buffer_.str("");
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
    CEDParagraph par;

    // empty paragraph
    CHECK_PAR("", par);
    parAddChar(par, 't');
    CHECK_PAR("<p>t</p>\n", par);

    par.clear();
    // explicit exports
    exp_->setSkipEmptyParagraphs(false);
    CHECK_PAR("<p></p>\n", par);

    parAddChar(par, 't');
    CHECK_PAR("<p>t</p>\n", par);

    par.setAlign(ALIGN_LEFT);
    CHECK_PAR("<p>t</p>\n", par);

    par.setAlign(ALIGN_RIGHT);
    CHECK_PAR("<p align=\"right\">t</p>\n", par);

    par.setAlign(ALIGN_CENTER);
    CHECK_PAR("<p align=\"center\">t</p>\n", par);

    par.setAlign(ALIGN_JUSTIFY);
    CHECK_PAR("<p align=\"justify\">t</p>\n", par);
}

void TestHtmlExporter::testExportLine() {
    CPPUNIT_ASSERT_EQUAL(true, exp_->skipEmptyLines());
    CEDLine line;

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
    line.setHardBreak(true);
    exp_->lines_left_in_paragraph_ = 2;
    exp_->exportLine(line);
    CPPUNIT_ASSERT_EQUAL(std::string("<br/>\n"), buf4.str());
}

void TestHtmlExporter::testExportCharacter() {
    // check special chars escaping
    ASSERT_CHAR_WRITE_CLEAR('&', "&amp;");
    ASSERT_CHAR_WRITE_CLEAR('>', "&gt;");
    ASSERT_CHAR_WRITE_CLEAR('<', "&lt;");
    ASSERT_CHAR_WRITE_CLEAR('"', "&quot;");
    ASSERT_CHAR_WRITE_CLEAR('\'', "&apos;");

    string chars("abcdefghijklmnopqstuvwxyz123456789!@#$%^*(-+");
    Letter lt;
    for (uint i = 0; i < chars.size(); i++) {
        lt.alternative_ = chars[i];
        c_->setAlternative(lt, 0);
        exp_->writeCharacter(cerr, *c_);
        CPPUNIT_ASSERT_EQUAL(string(1, chars[i]), buffer(exp_));
        clearBuffer(exp_);
    }
}

void TestHtmlExporter::testBold() {
    CEDParagraph par;

    exp_->formatOptions().useBold(true);

    parAddChar(par, 'a');
    CHECK_PAR("<p>a</p>\n", par);

    parAddChar(par, ' ');
    CHECK_PAR("<p>a </p>\n", par);

    parAddChar(par, 't', FONT_BOLD);
    CHECK_PAR("<p>a <b>t</b></p>\n", par);

    parAddChar(par, 'e', FONT_BOLD);
    CHECK_PAR("<p>a <b>te</b></p>\n", par);

    parAddChar(par, 's');
    CHECK_PAR("<p>a <b>te</b>s</p>\n", par);

    parAddChar(par, 't', FONT_BOLD);
    CHECK_PAR("<p>a <b>te</b>s<b>t</b></p>\n", par);

    // no bold
    exp_->formatOptions().useBold(false);

    CHECK_PAR("<p>a test</p>\n", par);
}

void TestHtmlExporter::testItalic() {
    exp_->formatOptions().useItalic(true);

    CEDParagraph par;

    parAddChar(par, 'a');
    CHECK_PAR("<p>a</p>\n", par);

    parAddChar(par, ' ');
    CHECK_PAR("<p>a </p>\n", par);

    parAddChar(par, 't', FONT_ITALIC);
    CHECK_PAR("<p>a <i>t</i></p>\n", par);

    parAddChar(par, 'e', FONT_ITALIC);
    CHECK_PAR("<p>a <i>te</i></p>\n", par);

    parAddChar(par, 's');
    CHECK_PAR("<p>a <i>te</i>s</p>\n", par);

    parAddChar(par, 't', FONT_ITALIC);
    CHECK_PAR("<p>a <i>te</i>s<i>t</i></p>\n", par);

    // no italic
    exp_->formatOptions().useItalic(false);
    CHECK_PAR("<p>a test</p>\n", par);
}

void TestHtmlExporter::testUnderlined() {
    CEDParagraph par;

    parAddChar(par, 'a');
    parAddChar(par, ' ');
    CHECK_PAR("<p>a </p>\n", par);

    parAddChar(par, 't', FONT_UNDERLINE);
    CHECK_PAR("<p>a <u>t</u></p>\n", par);

    parAddChar(par, 'e', FONT_UNDERLINE);
    CHECK_PAR("<p>a <u>te</u></p>\n", par);

    parAddChar(par, 's');
    CHECK_PAR("<p>a <u>te</u>s</p>\n", par);

    parAddChar(par, 't', FONT_UNDERLINE);
    CHECK_PAR("<p>a <u>te</u>s<u>t</u></p>\n", par);
}

void TestHtmlExporter::testSub() {
    CEDParagraph par;

    parAddChar(par, 'a');
    parAddChar(par, ' ');
    CHECK_PAR("<p>a </p>\n", par);

    parAddChar(par, 's', FONT_SUB);
    CHECK_PAR("<p>a <sub>s</sub></p>\n", par);

    parAddChar(par, 'u', FONT_SUB);
    CHECK_PAR("<p>a <sub>su</sub></p>\n", par);

    parAddChar(par, 'b');
    CHECK_PAR("<p>a <sub>su</sub>b</p>\n", par);
}

void TestHtmlExporter::testSuper() {
    CEDParagraph par;

    parAddChar(par, 'a');
    parAddChar(par, ' ');
    CHECK_PAR("<p>a </p>\n", par);

    parAddChar(par, 's', FONT_SUPER);
    CHECK_PAR("<p>a <sup>s</sup></p>\n", par);

    parAddChar(par, 'u', FONT_SUPER);
    CHECK_PAR("<p>a <sup>su</sup></p>\n", par);

    parAddChar(par, 'p');
    CHECK_PAR("<p>a <sup>su</sup>p</p>\n", par);
}

void TestHtmlExporter::testMixed() {
    CEDParagraph par;

    parAddChar(par, 'm');
    CHECK_PAR("<p>m</p>\n", par);

    parAddChar(par, 'i', FONT_BOLD);
    parAddChar(par, 'x', FONT_BOLD);
    CHECK_PAR("<p>m<b>ix</b></p>\n", par);

    parAddChar(par, 'e', FONT_ITALIC);
    parAddChar(par, 'd', FONT_ITALIC);
    parAddChar(par, ' ');
    CHECK_PAR("<p>m<b>ix</b><i>ed</i> </p>\n", par);

    parAddChar(par, 's', FONT_UNDERLINE);
    parAddChar(par, 't', FONT_UNDERLINE);
    CHECK_PAR("<p>m<b>ix</b><i>ed</i> <u>st</u></p>\n", par);

    parAddChar(par, 'y', FONT_SUB);
    parAddChar(par, 'l', FONT_SUB);
    CHECK_PAR("<p>m<b>ix</b><i>ed</i> <u>st</u><sub>yl</sub></p>\n", par);

    parAddChar(par, 'e', FONT_SUPER);
    parAddChar(par, 's', FONT_SUPER);
    CHECK_PAR("<p>m<b>ix</b><i>ed</i> <u>st</u><sub>yl</sub><sup>es</sup></p>\n", par);

    par.clear();

    parAddChar(par, 'm', FONT_BOLD);
    CHECK_PAR("<p><b>m</b></p>\n", par);

    parAddChar(par, 'i', FONT_BOLD | FONT_ITALIC);
    CHECK_PAR("<p><b>m</b><b><i>i</i></b></p>\n", par);

    parAddChar(par, 'x', FONT_ITALIC);
    CHECK_PAR("<p><b>m</b><b><i>i</i></b><i>x</i></p>\n", par);

    par.clear();
    parAddChar(par, 'm', FONT_BOLD | FONT_ITALIC | FONT_UNDERLINE | FONT_SUB | FONT_SUPER);
    CHECK_PAR("<p><b><i><u><sub><sup>m</sup></sub></u></i></b></p>\n", par);

    par.clear();
    parAddChar(par, 'm', FONT_BOLD | FONT_ITALIC | FONT_UNDERLINE | FONT_SUB);
    CHECK_PAR("<p><b><i><u><sub>m</sub></u></i></b></p>\n", par);

    par.clear();
    parAddChar(par, 'm', FONT_BOLD | FONT_ITALIC | FONT_SUB);
    CHECK_PAR("<p><b><i><sub>m</sub></i></b></p>\n", par);

    par.clear();
    parAddChar(par, 'm', FONT_ITALIC | FONT_SUB);
    CHECK_PAR("<p><i><sub>m</sub></i></p>\n", par);

    par.clear();
    parAddChar(par, 'm', FONT_SUB);
    CHECK_PAR("<p><sub>m</sub></p>\n", par);

}

void TestHtmlExporter::testExportPicture() {
    CEDPicture pict;

#ifndef EXPORTER_TEST_IMAGE_DIR
#define EXPORTER_TEST_IMAGE_DIR "./"
#endif

    pict.setImage(ImageLoaderFactory::instance().load(EXPORTER_TEST_IMAGE_DIR "test_in.bmp"));

    exp_->setOutputPictureDir("output");
    exp_->exportPicture(pict);

    CPPUNIT_ASSERT_EQUAL(std::string("<img alt=\"\" height=\"0\" src=\"output/image_0.png\" width=\"0\"/>\n"),
            exp_->lineBuffer().str());
}

void TestHtmlExporter::testExportColumn() {
    CEDColumn col;
    CEDParagraph * par1 = new CEDParagraph;
    CEDParagraph * par2 = new CEDParagraph;
    col.addElement(par1);
    col.addElement(par2);

    parAddChar(*par1, 't', FONT_BOLD);
    parAddChar(*par2, 'e', FONT_BOLD);

    exp_->exportColumn(col);
    CHECK_COL("<p><b>t</b></p>\n<p><b>e</b></p>\n", col);
}

void TestHtmlExporter::testWriteAlternatives() {
    exp_->formatOptions().setShowAlternatives(true);
    CEDParagraph par;

    CEDChar * c = parAddChar(par, 'a');

    CHECK_PAR("<p>a</p>\n", par);

    c->addAlternative('b');
    CHECK_PAR("<p><span title=\"Alternatives: b\" class=\"has_alternative\">a</span></p>\n", par);

    c->setFontStyle(FONT_UNDERLINE);
    CHECK_PAR("<p><u><span title=\"Alternatives: b\" class=\"has_alternative\">a</span></u></p>\n", par);
}

