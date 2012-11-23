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
#include "testcedline.h"
#include "../test_common.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCedLine);
#define private public
#include <iostream>
#include <fstream>
#include "common/tostring.h"
#include <ced/cedchar.h>
#include <ced/cedline.h>
#include <ced/cedpicture.h>
#include <ced/cedarchive.h>
using namespace cf;

void TestCedLine::testEndsWithHyphen() {
    CEDLine ln;

    CPPUNIT_ASSERT(!ln.endsWithHyphen());

    ln.addElement(new CEDChar);
    CPPUNIT_ASSERT(!ln.endsWithHyphen());

    CEDChar * chr = new CEDChar;
    chr->addAlternative('-');
    ln.addElement(chr);

    CPPUNIT_ASSERT(ln.endsWithHyphen());

    ln.addElement(new CEDChar);
    CPPUNIT_ASSERT(!ln.endsWithHyphen());
}

void TestCedLine::testInit() {
    CEDLine ln;
    CPPUNIT_ASSERT(0 == ln.elementCount());
    CPPUNIT_ASSERT_THROW(ln.first(), std::out_of_range);
}

void TestCedLine::testInsertChar() {
    CEDLine ln;
    CEDChar * chr = ln.insertChar();

    CPPUNIT_ASSERT(chr);
    CPPUNIT_ASSERT_EQUAL(size_t(1), ln.elementCount());
    CPPUNIT_ASSERT_EQUAL(chr, ln.first());
    CPPUNIT_ASSERT_EQUAL(chr, ln.charAt(0));

    CEDChar * chr2 = ln.insertChar();
    CPPUNIT_ASSERT(chr2);
    CPPUNIT_ASSERT_EQUAL(size_t(2), ln.elementCount());
    CPPUNIT_ASSERT_EQUAL(chr, ln.charAt(0));
    CPPUNIT_ASSERT_EQUAL(chr2, ln.charAt(1));
    CPPUNIT_ASSERT_EQUAL(chr, ln.first());

    CEDChar * chr3 = ln.insertChar();
    CPPUNIT_ASSERT(chr3);
    CPPUNIT_ASSERT_EQUAL(size_t(3), ln.elementCount());
    CPPUNIT_ASSERT_EQUAL(chr, ln.charAt(0));
    CPPUNIT_ASSERT_EQUAL(chr2, ln.charAt(1));
    CPPUNIT_ASSERT_EQUAL(chr3, ln.charAt(2));
    CPPUNIT_ASSERT_EQUAL(chr, ln.first());
}

void TestCedLine::testCharAt() {
    CEDLine ln;
    CEDChar * chr = ln.insertChar();
    CPPUNIT_ASSERT_EQUAL(chr, ln.charAt(0));
    CPPUNIT_ASSERT_THROW(ln.charAt(-1), std::out_of_range);
    CPPUNIT_ASSERT_THROW(ln.charAt(19), std::out_of_range);
}

void TestCedLine::testSerialize() {
#ifdef CF_SERIALIZE
    const char * fname = "serialize_cedline.txt";
    CEDLine ln;
    ln.setHardBreak(true);
    ln.setDefaultFontHeight(11);

    ln.insertChar(new CEDChar);
    ln.charAt(0)->setFontHeight(12);
    ln.charAt(0)->setColor(Color(0, 100, 200));

    CEDPicture * im = new CEDPicture;
    im->setImage(new Image(new uchar[10], 10, Image::AllocatorNew));
    im->image()->setFileName("CEDLine");
    ln.addImage(im);

    writeToTextArchive(fname, ln);

    CEDLine new_line;
    readFromTextArchive(fname, new_line);

    CPPUNIT_ASSERT_EQUAL(ln.elementCount(), new_line.elementCount());
    CPPUNIT_ASSERT_EQUAL(ln.defaultFontHeight(), new_line.defaultFontHeight());
    CPPUNIT_ASSERT_EQUAL(ln.hardBreak(), new_line.hardBreak());
    CPPUNIT_ASSERT(new_line.charAt(0)->fontHeight() == 12);
#endif
}

void TestCedLine::testSerializeXml() {
#ifdef CF_SERIALIZE
    const char * fname = "serialize_cedline.xml";
    const Color ln_color(0, 100, 200);
    CEDLine ln;
    ln.setHardBreak(true);
    ln.setDefaultFontHeight(11);

    ln.insertChar(new CEDChar);
    ln.charAt(0)->setFontHeight(12);
    ln.charAt(0)->setColor(ln_color);

    CEDPicture * im = new CEDPicture;
    im->setImage(new Image(new uchar[10], 10, Image::AllocatorNew));
    im->image()->setFileName("CEDLine");
    ln.addImage(im);

    writeToXmlArchive(fname, "cedline", ln);

    CEDLine new_line;
    readFromXmlArchive(fname, "cedline", new_line);

    CPPUNIT_ASSERT_EQUAL(ln.elementCount(), new_line.elementCount());
    CPPUNIT_ASSERT_EQUAL(ln.defaultFontHeight(), new_line.defaultFontHeight());
    CPPUNIT_ASSERT_EQUAL(ln.hardBreak(), new_line.hardBreak());

    // char checks
    CPPUNIT_ASSERT(new_line.charAt(0)->fontHeight() == 12);
    CPPUNIT_ASSERT_EQUAL(new_line.charAt(0)->color(), ln_color);

    //picture checks
    CEDPicture * new_pict = dynamic_cast<CEDPicture*>(ln.elementAt(1));
    CPPUNIT_ASSERT(new_pict);
    CPPUNIT_ASSERT_EQUAL(new_pict->image()->fileName(), im->image()->fileName());
    CPPUNIT_ASSERT_EQUAL(new_pict->align(), im->align());
#endif
}

void TestCedLine::testClone()
{
    CEDLine line;
    line.setHardBreak(true);
    line.setDefaultFontHeight(100);

    CEDLine * line_copy = line.clone();
    CPPUNIT_ASSERT(line_copy);
    CPPUNIT_ASSERT_EQUAL(line.hardBreak(), line_copy->hardBreak());
    CPPUNIT_ASSERT_EQUAL(line.defaultFontHeight(), line_copy->defaultFontHeight());
    delete line_copy;
}
