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
#include <iostream>
#include <fstream>
#include "testcedchar.h"
#include "../test_common.h"
#include <common/tostring.h>
#include <ced/cedchar.h>
CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDChar);

using namespace cf;

void TestCEDChar::testInit() {
}

void TestCEDChar::testIsHyphen() {
    CEDChar chr;

    CPPUNIT_ASSERT(!chr.isHyphen());

    chr.addAlternative(' ');
    CPPUNIT_ASSERT(!chr.isHyphen());
    chr.setAlternative('-', 0);
    CPPUNIT_ASSERT(chr.isHyphen());
}

void TestCEDChar::testSerialize() {
#ifdef CF_SERIALIZE
    const char * FNAME = "serialize_cedchar.txt";
    CEDChar chr;
    chr.addAlternative(Letter('a', 128));
    chr.setFontHeight(11);
    chr.setBackgroundColor(Color(255, 0, 0));
    chr.setColor(Color(0, 0, 255));
    chr.setBoundingRect(Rect(Point(1, 2), Point(3, 4)));

    writeToTextArchive(FNAME, chr);

    CEDChar new_chr;
    readFromTextArchive(FNAME, new_chr);

    CPPUNIT_ASSERT_EQUAL(chr.alternativeCount(), new_chr.alternativeCount());
    CPPUNIT_ASSERT_EQUAL(chr.fontHeight(), new_chr.fontHeight());
    CPPUNIT_ASSERT_EQUAL(chr.backgroundColor(), new_chr.backgroundColor());
    CPPUNIT_ASSERT_EQUAL(chr.color(), new_chr.color());
    CPPUNIT_ASSERT_EQUAL(chr.boundingRect(), new_chr.boundingRect());
#endif
}

void TestCEDChar::testSerializeXml() {
#ifdef CF_SERIALIZE
    const char * FNAME = "serialize_cedchar.xml";
    CEDChar chr;
    chr.addAlternative(Letter('a', 128));
    chr.setFontHeight(11);
    chr.setFontStyle(1);
    chr.setFontNumber(2);
    chr.setBackgroundColor(Color(255, 0, 0));
    chr.setColor(Color(0, 0, 255));
    chr.setBoundingRect(Rect(Point(1, 2), Point(3, 4)));

    writeToXmlArchive(FNAME, "cedchar", chr);

    CEDChar new_chr;
    readFromXmlArchive(FNAME, "cedchar", new_chr);

    CPPUNIT_ASSERT_EQUAL(chr.fontHeight(), new_chr.fontHeight());
    CPPUNIT_ASSERT_EQUAL(chr.fontStyle(), new_chr.fontStyle());
    CPPUNIT_ASSERT_EQUAL(chr.fontNumber(), new_chr.fontNumber());
    CPPUNIT_ASSERT_EQUAL(chr.backgroundColor(), new_chr.backgroundColor());
    CPPUNIT_ASSERT_EQUAL(chr.color(), new_chr.color());
    CPPUNIT_ASSERT_EQUAL(chr.boundingRect(), new_chr.boundingRect());
    CPPUNIT_ASSERT_EQUAL(chr.alternativeCount(), new_chr.alternativeCount());

#endif
}

void TestCEDChar::testClone()
{
    CEDChar chr;
    // char properties
    chr.addAlternative(Letter('a', 128));
    chr.setFontHeight(133);
    chr.setFontStyle(1);
    chr.setFontNumber(2);
    chr.setFontLanguage(LANGUAGE_DUTCH);
    // element properties
    chr.setParent(NULL);
    chr.setBackgroundColor(Color(255, 0, 0));
    chr.setColor(Color(0, 0, 255));
    chr.setBoundingRect(Rect(Point(1, 2), Point(3, 4)));

    CEDChar * ch_copy = chr.clone();
    CPPUNIT_ASSERT(ch_copy);
    CPPUNIT_ASSERT_EQUAL(chr.fontHeight(), ch_copy->fontHeight());
    CPPUNIT_ASSERT_EQUAL(chr.fontStyle(), ch_copy->fontStyle());
    CPPUNIT_ASSERT_EQUAL(chr.fontNumber(), ch_copy->fontNumber());
    CPPUNIT_ASSERT_EQUAL(chr.fontLanguage(), ch_copy->fontLanguage());
    // element data
    CPPUNIT_ASSERT_EQUAL(chr.parent(), ch_copy->parent());
    CPPUNIT_ASSERT_EQUAL(chr.backgroundColor(), ch_copy->backgroundColor());
    CPPUNIT_ASSERT_EQUAL(chr.color(), ch_copy->color());
    CPPUNIT_ASSERT_EQUAL(chr.boundingRect(), ch_copy->boundingRect());
    delete ch_copy;

    // test parent
    CEDChar ch1;
    ch1.setParent(&chr);
    CEDChar * ch1_copy = ch1.clone();
    CPPUNIT_ASSERT(ch1_copy);
    CPPUNIT_ASSERT_EQUAL(ch1.parent(), ch1_copy->parent());
    delete ch1_copy;
}
