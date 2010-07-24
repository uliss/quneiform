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
#include "testtextexporter.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestTextExporter);
#include "ced/cedchar.h"
#include "ced/cedline.h"
#include "ced/cedpage.h"
#include "ced/cedsection.h"
#include "ced/cedparagraph.h"
#include "ced/cedpicture.h"

#define protected public
#define private public
#include "export/textexporter.h"

using namespace CIF;

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

    return lineAddChar(p.lineAt(p.lineCount() - 1), letter, style);
}

#define BUFFER_CLEAR() {\
	buffer_.str("");\
	exp_->line_buffer_.str("");\
}

#define CHECK_BUFFER(s, buf) {\
    CPPUNIT_ASSERT_EQUAL(std::string(s), buf.str());\
}

#define CHECK_PAR(s, par) {\
    BUFFER_CLEAR()\
    exp_->exportParagraph(par);\
    CHECK_BUFFER(s, buffer_);\
}

#define CHECK_LINE(s, line) {\
    BUFFER_CLEAR();\
    exp_->exportLine(line);\
    CHECK_BUFFER(s, buffer_);\
}

void TestTextExporter::setUp() {
    FormatOptions opts;
    exp_ = new TextExporter(NULL, opts);
    exp_->setOutputStream(&buffer_);
    buffer_.str("");
}

void TestTextExporter::tearDown() {
    delete exp_;
}

void TestTextExporter::testExportLine() {
    CEDLine ln;

    CHECK_LINE("", ln);

    lineAddChar(&ln, 't');
    CHECK_LINE("t", ln);

    exp_->formatOptions().setPreserveLineBreaks(true);
    CHECK_LINE("t", ln);
    exp_->formatOptions().setPreserveLineBreaks(false);
    CHECK_LINE("t", ln);

    lineAddChar(&ln, '-');
    CHECK_LINE("t-", ln);

    ln.setHardBreak(true);
    CHECK_LINE("t-", ln);
    ln.setHardBreak(false);
    CHECK_LINE("t-", ln);

    CEDParagraph par;
    parAddChar(par, 't');
    parAddChar(par, 'e');
    parAddChar(par, '-');
    CHECK_PAR("te-\n", par);

    par.addElement(new CEDLine);
    CHECK_PAR("te\n", par);
    parAddChar(par, 's');
    parAddChar(par, 't');
    CHECK_PAR("test\n", par);

    par.clear();

    parAddChar(par, 't');
    parAddChar(par, 'h');
    parAddChar(par, 'e');
    CHECK_PAR("the\n", par);

    par.addElement(new CEDLine);
    // space added before last line
    CHECK_PAR("the \n", par);

    parAddChar(par, 'c');
    parAddChar(par, 'a');
    parAddChar(par, 'p');
    CHECK_PAR("the cap\n", par);
}

void TestTextExporter::testExportPage() {
    CEDPage page;

    exp_->exportPage(page);
    CHECK_BUFFER("\n", buffer_);
}

void TestTextExporter::testExportParagraph() {
    CEDParagraph par;

    CHECK_PAR("", par);

    exp_->setSkipEmptyParagraphs(true);
    CHECK_PAR("", par);

    exp_->setSkipEmptyParagraphs(false);
    CHECK_PAR("\n", par);

    parAddChar(par, 't');
    parAddChar(par, 'h');
    parAddChar(par, 'e');
    CHECK_PAR("the\n", par);

    par.addElement(new CEDLine);
    // space added before last line
    CHECK_PAR("the \n", par);

    parAddChar(par, 'c');
    parAddChar(par, 'a');
    parAddChar(par, 'p');
    parAddChar(par, '-');
    CHECK_PAR("the cap-\n", par);

    exp_->formatOptions().setPreserveLineBreaks(true);
    par.lineAt(0)->setHardBreak(true);
    CHECK_PAR("the\ncap-\n", par);
    par.lineAt(0)->setHardBreak(false);
    CHECK_PAR("the\ncap-\n", par);

    exp_->formatOptions().setPreserveLineBreaks(false);
    par.lineAt(0)->setHardBreak(true);
    CHECK_PAR("the\ncap-\n", par);
    par.lineAt(0)->setHardBreak(false);
    CHECK_PAR("the cap-\n", par);

    par.clear();
    parAddChar(par, 'h');
    parAddChar(par, 'y');
    parAddChar(par, '-');
    par.addElement(new CEDLine);
    parAddChar(par, 'p');
    parAddChar(par, 'h');
    parAddChar(par, 'e');
    parAddChar(par, 'n');
    parAddChar(par, '-');

    exp_->formatOptions().setPreserveLineBreaks(true);
    par.lineAt(0)->setHardBreak(true);
    CHECK_PAR("hy-\nphen-\n", par);
    par.lineAt(0)->setHardBreak(false);
    CHECK_PAR("hy-\nphen-\n", par);

    exp_->formatOptions().setPreserveLineBreaks(false);
    par.lineAt(0)->setHardBreak(true);
    CHECK_PAR("hy-\nphen-\n", par);
    par.lineAt(0)->setHardBreak(false);
    CHECK_PAR("hyphen-\n", par);
}

void TestTextExporter::testExportPicture() {
    CEDPicture pict;
    pict.setBoundingRect(Rect(Point(), 10, 20));

    exp_->exportPicture(pict);
    CHECK_BUFFER("[picture: 10x20]\n", buffer_);
}

void TestTextExporter::testExportSection() {
    CEDSection sect;

    exp_->exportSection(sect);
    CHECK_BUFFER("\n", buffer_);
}
