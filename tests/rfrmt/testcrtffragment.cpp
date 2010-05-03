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
#include <stdexcept>
#include "testcrtffragment.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCRtfFragment);
#define private public
#include "rfrmt/crtffragment.h"
#include "rfrmt/crtfstring.h"
#include "rfrmt/crtfword.h"
#include "rfrmt/crtfchar.h"

using namespace CIF;

#define INIT_STR(str, left, right) \
        { \
    CPPUNIT_ASSERT(str->empty());\
    str->addWord(new CRtfWord);\
    str->lastWord()->addChar(new CRtfChar('C'));\
    Rect r;\
    r.setLeft(left);\
    r.setRight(right);\
    str->firstChar()->setIdealRect(r);\
    str->firstChar()->setRealRect(r);\
    CPPUNIT_ASSERT_EQUAL(left, str->leftBorder());\
    CPPUNIT_ASSERT_EQUAL(right, str->rightBorder());\
        }

#define FR_ADD_STR(fr, left, right) \
        {\
    CRtfString * str = new CRtfString;\
    INIT_STR(str, left, right);\
    fr.addString(str);\
        }

void TestCRtfFragment::testInit() {
    CRtfFragment fr;
    CPPUNIT_ASSERT(fr.stringCount() == 0);
    CPPUNIT_ASSERT_THROW(fr.stringAt(0), std::out_of_range);
    CPPUNIT_ASSERT_THROW(fr.firstString(), std::out_of_range);
}

void TestCRtfFragment::testAddString() {
    CRtfFragment fr;
    CPPUNIT_ASSERT(fr.stringCount() == 0);
    CRtfString * str = new CRtfString;
    fr.addString(str);
    CPPUNIT_ASSERT(fr.stringCount() == 1);
    CPPUNIT_ASSERT_EQUAL(str, fr.stringAt(0));
    CPPUNIT_ASSERT_EQUAL(str, fr.firstString());
}

void TestCRtfFragment::testClearStrings() {
    CRtfFragment fr;
    CPPUNIT_ASSERT(fr.stringCount() == 0);
    fr.clearStrings();
    CPPUNIT_ASSERT(fr.stringCount() == 0);
    fr.addString(new CRtfString);
    CPPUNIT_ASSERT(fr.stringCount() == 1);
    fr.addString(new CRtfString);
    CPPUNIT_ASSERT(fr.stringCount() == 2);
    fr.clearStrings();
    CPPUNIT_ASSERT(fr.stringCount() == 0);
}

void TestCRtfFragment::testFontSizePenalty() {
    CRtfFragment fr;
    // penalty for empty fragment
    CPPUNIT_ASSERT(fr.fontSizePenalty(0) == 0);
}

void TestCRtfFragment::testCharCount() {
    CRtfFragment fr;
    CPPUNIT_ASSERT(fr.charCount() == 0);
    CRtfString * str = new CRtfString;
    fr.addString(str);
    CPPUNIT_ASSERT(fr.charCount() == 0);
    CRtfWord * word = new CRtfWord;
    str->addWord(word);
    CPPUNIT_ASSERT(fr.charCount() == 0);
    CRtfChar * chr = new CRtfChar('t');
    word->addChar(chr);
    CPPUNIT_ASSERT(str->charCount() == 1);
    CPPUNIT_ASSERT(fr.charCount() == 1);
    str = new CRtfString;
    fr.addString(str);
    CPPUNIT_ASSERT(str->charCount() == 0);
    CPPUNIT_ASSERT(fr.charCount() == 1);
    word = new CRtfWord;
    str->addWord(word);
    CPPUNIT_ASSERT(fr.charCount() == 1);
    chr = new CRtfChar('e');
    word->addChar(chr);
    CPPUNIT_ASSERT(str->charCount() == 1);
    CPPUNIT_ASSERT(fr.charCount() == 2);
}

void TestCRtfFragment::testCharTotalLength() {
    CRtfFragment fr;
    CPPUNIT_ASSERT(fr.charTotalLength() == 0);

    FR_ADD_STR(fr, 0, 20);
    CPPUNIT_ASSERT_EQUAL(20, fr.charTotalLength());
}

void TestCRtfFragment::testCalcMaxCharDistance() {
    CRtfFragment fr;
    CPPUNIT_ASSERT(fr.max_char_distance_ == 0);
    fr.calcMaxCharDistance();
    static const int DEFAULT_DISTANCE = 10;
    CPPUNIT_ASSERT(fr.max_char_distance_ == DEFAULT_DISTANCE);

    fr.addString(new CRtfString);
    fr.calcMaxCharDistance();
    CPPUNIT_ASSERT(fr.max_char_distance_ == DEFAULT_DISTANCE);

    fr.firstString()->addWord(new CRtfWord);
    fr.calcMaxCharDistance();
    CPPUNIT_ASSERT(fr.max_char_distance_ == DEFAULT_DISTANCE);

    fr.firstString()->lastWord()->addChar(new CRtfChar('t'));
    fr.calcMaxCharDistance();
    CPPUNIT_ASSERT(fr.max_char_distance_ == 0);

    Rect r;
    r.setWidth(100);
    fr.firstString()->firstChar()->setRealRect(r);
    fr.calcMaxCharDistance();
    CPPUNIT_ASSERT(fr.max_char_distance_ == 100);

    fr.firstString()->lastWord()->addChar(new CRtfChar('e'));
    fr.calcMaxCharDistance();
    CPPUNIT_ASSERT(fr.max_char_distance_ == 50);

    r.setWidth(80);
    fr.firstString()->lastWord()->addChar(new CRtfChar('e'));
    fr.firstString()->lastChar()->setRealRect(r);
    fr.calcMaxCharDistance();
    CPPUNIT_ASSERT(fr.max_char_distance_ == 60);
}

void TestCRtfFragment::testMinStringLeftBorder() {
    CRtfFragment fr;
    CPPUNIT_ASSERT_THROW(fr.minStringLeftBorder(), std::out_of_range);

    CRtfString * str1 = new CRtfString;
    str1->addWord(new CRtfWord);
    str1->lastWord()->addChar(new CRtfChar);

    CRtfString * str2 = new CRtfString;
    str2->addWord(new CRtfWord);
    str2->lastWord()->addChar(new CRtfChar);

    CRtfString * str3 = new CRtfString;
    str3->addWord(new CRtfWord);
    str3->lastWord()->addChar(new CRtfChar);

    fr.addString(str1);
    fr.addString(str2);
    fr.addString(str3);

    CPPUNIT_ASSERT(fr.minStringLeftBorder() == 0);

    Rect r;

    r.setLeft(10);
    str1->firstChar()->setIdealRect(r);
    CPPUNIT_ASSERT_EQUAL(0, fr.minStringLeftBorder());

    r.setLeft(20);
    str2->firstChar()->setIdealRect(r);
    CPPUNIT_ASSERT_EQUAL(0, fr.minStringLeftBorder());

    r.setLeft(30);
    str3->firstChar()->setIdealRect(r);
    CPPUNIT_ASSERT_EQUAL(10, fr.minStringLeftBorder());

    r.setLeft(-10);
    str2->firstChar()->setIdealRect(r);
    CPPUNIT_ASSERT_EQUAL(-10, fr.minStringLeftBorder());
}

void TestCRtfFragment::testMaxStringRightBorder() {
    CRtfFragment fr;
    CPPUNIT_ASSERT_THROW(fr.maxStringRightBorder(), std::out_of_range);

    CRtfString * str1 = new CRtfString;
    str1->addWord(new CRtfWord);
    str1->lastWord()->addChar(new CRtfChar);

    CRtfString * str2 = new CRtfString;
    str2->addWord(new CRtfWord);
    str2->lastWord()->addChar(new CRtfChar);

    CRtfString * str3 = new CRtfString;
    str3->addWord(new CRtfWord);
    str3->lastWord()->addChar(new CRtfChar);

    fr.addString(str1);
    fr.addString(str2);
    fr.addString(str3);

    CPPUNIT_ASSERT(fr.maxStringRightBorder() == 0);

    Rect r;

    r.setRight(10);
    str1->firstChar()->setIdealRect(r);
    CPPUNIT_ASSERT_EQUAL(10, fr.maxStringRightBorder());

    r.setRight(20);
    str2->firstChar()->setIdealRect(r);
    CPPUNIT_ASSERT_EQUAL(20, fr.maxStringRightBorder());

    r.setRight(30);
    str3->firstChar()->setIdealRect(r);
    CPPUNIT_ASSERT_EQUAL(30, fr.maxStringRightBorder());

    r.setLeft(-10);
    str2->firstChar()->setIdealRect(r);
    CPPUNIT_ASSERT_EQUAL(30, fr.maxStringRightBorder());
}

void TestCRtfFragment::testAdjustParagraph() {
    CRtfFragment fr;
    fr.adjustParagraph(0);
    fr.addString(new CRtfString);
    CPPUNIT_ASSERT_EQUAL(0, fr.firstString()->marginTop());
    CPPUNIT_ASSERT(!fr.firstString()->isParagraphBegin());
    fr.adjustParagraph(100);
    CPPUNIT_ASSERT_EQUAL(100, fr.firstString()->marginTop());
    CPPUNIT_ASSERT(fr.firstString()->isParagraphBegin());
    fr.addString(new CRtfString);
    fr.stringAt(1)->setTopMargin(10);
    fr.stringAt(1)->setParagraphBegin(true);
    fr.adjustParagraph(100);
    CPPUNIT_ASSERT_EQUAL(100, fr.firstString()->marginTop());
    CPPUNIT_ASSERT(fr.firstString()->isParagraphBegin());
    CPPUNIT_ASSERT_EQUAL(0, fr.stringAt(1)->marginTop());
    CPPUNIT_ASSERT(!fr.stringAt(1)->isParagraphBegin());

    // hyphens
}

#define FR_LEQUAL(fr, value) \
    CPPUNIT_ASSERT(fr.m_CountLeftEqual == value);

#define FR_REQUAL(fr, value) \
    CPPUNIT_ASSERT_EQUAL((int)value, (int)fr.m_CountRightEqual);

#define FR_CEQUAL(fr, value) \
	CPPUNIT_ASSERT(fr.m_CountCentreEqual == value);

#define FR_CALC(fr) \
		{\
    fr.calcFragmentBorders();\
    fr.adjustStringIndents();\
    fr.calcStringEndsEqual();\
		}

void TestCRtfFragment::testCalcStringEndsEqual() {
    CRtfFragment fr;
    fr.max_char_distance_ = 10;
    FR_CALC(fr);

    FR_LEQUAL(fr, 0);
    FR_REQUAL(fr, 0);
    FR_CEQUAL(fr, 0);

    FR_ADD_STR(fr, 0, 200);
    FR_CALC(fr);
    CPPUNIT_ASSERT(!fr.lastString()->isEqualLeft());
    CPPUNIT_ASSERT(!fr.lastString()->isEqualRight());
    CPPUNIT_ASSERT(!fr.lastString()->isEqualCenter());
    FR_LEQUAL(fr, 0);
    FR_REQUAL(fr, 0);
    FR_CEQUAL(fr, 0);

    FR_ADD_STR(fr, 1, 50);
    FR_CALC(fr);
    CPPUNIT_ASSERT(fr.lastString()->isEqualLeft());
    CPPUNIT_ASSERT(!fr.lastString()->isEqualRight());
    CPPUNIT_ASSERT(!fr.lastString()->isEqualCenter());
    FR_LEQUAL(fr, 2);
    FR_REQUAL(fr, 0);
    FR_CEQUAL(fr, 0);

    FR_ADD_STR(fr, 2, 51);
    FR_CALC(fr);
    CPPUNIT_ASSERT(fr.lastString()->isEqualLeft());
    CPPUNIT_ASSERT(fr.lastString()->isEqualRight());
    CPPUNIT_ASSERT(!fr.lastString()->isEqualCenter());
    FR_LEQUAL(fr, 3);
    // TODO check value "1"...
    FR_REQUAL(fr, 1);
    FR_CEQUAL(fr, 0);

    FR_ADD_STR(fr, 13, 40);
    FR_CALC(fr);
    CPPUNIT_ASSERT(!fr.lastString()->isEqualLeft());
    CPPUNIT_ASSERT(!fr.lastString()->isEqualRight());
    CPPUNIT_ASSERT(fr.lastString()->isEqualCenter());
    FR_LEQUAL(fr, 3);
    FR_REQUAL(fr, 1);
    FR_CEQUAL(fr, 1);
}

void TestCRtfFragment::testCalcFragmentBorders() {
    CRtfFragment fr;

    FR_ADD_STR(fr, 0, 100);
    fr.calcFragmentBorders();
    CPPUNIT_ASSERT_EQUAL(0, fr.left_border_);
    CPPUNIT_ASSERT_EQUAL(100, fr.right_border_);

    FR_ADD_STR(fr, -10, 40);
    fr.calcFragmentBorders();
    CPPUNIT_ASSERT_EQUAL(-10, fr.left_border_);
    CPPUNIT_ASSERT_EQUAL(100, fr.right_border_);

    FR_ADD_STR(fr, 50, 400);
    fr.calcFragmentBorders();
    CPPUNIT_ASSERT_EQUAL(-10, fr.left_border_);
    CPPUNIT_ASSERT_EQUAL(400, fr.right_border_);
}

void TestCRtfFragment::testAdjustStringIndents() {
    CRtfFragment fr;

    FR_ADD_STR(fr, 0, 100);
    fr.calcFragmentBorders();
    fr.adjustStringIndents();
    CPPUNIT_ASSERT_EQUAL(0, fr.firstString()->leftIndent());
    CPPUNIT_ASSERT_EQUAL(0, fr.firstString()->rightIndent());

    FR_ADD_STR(fr, 0, 70);
    fr.calcFragmentBorders();
    fr.adjustStringIndents();
    CPPUNIT_ASSERT_EQUAL(0, fr.lastString()->leftIndent());
    CPPUNIT_ASSERT_EQUAL(30, fr.lastString()->rightIndent());

    FR_ADD_STR(fr, 10, 20);
    fr.calcFragmentBorders();
    fr.adjustStringIndents();
    CPPUNIT_ASSERT_EQUAL(10, fr.lastString()->leftIndent());
    CPPUNIT_ASSERT_EQUAL(80, fr.lastString()->rightIndent());
}

void TestCRtfFragment::testCountEqualAlign() {
    CRtfFragment fr;

#define COUNT_EQUAL(count, align) \
        CPPUNIT_ASSERT_EQUAL((count), fr.countEqualAlign(fr.strings_.begin(), fr.strings_.end(), (align)));

    COUNT_EQUAL(0, FORMAT_ALIGN_LEFT);
    COUNT_EQUAL(0, FORMAT_ALIGN_RIGHT);
    COUNT_EQUAL(0, FORMAT_ALIGN_JUSTIFY);
    COUNT_EQUAL(0, FORMAT_ALIGN_CENTER);

    FR_ADD_STR(fr, 0, 100);
    fr.lastString()->setEqualLeft(true);
    fr.lastString()->setEqualRight(true);

    COUNT_EQUAL(2, FORMAT_ALIGN_LEFT);
    COUNT_EQUAL(2, FORMAT_ALIGN_RIGHT);
    COUNT_EQUAL(2, FORMAT_ALIGN_JUSTIFY);
    COUNT_EQUAL(0, FORMAT_ALIGN_CENTER);

    FR_ADD_STR(fr, 0, 100);
    fr.lastString()->setEqualCenter(true);
    fr.lastString()->setEqualRight(true);
    COUNT_EQUAL(2, FORMAT_ALIGN_LEFT);
    COUNT_EQUAL(2, FORMAT_ALIGN_RIGHT);
    COUNT_EQUAL(2, FORMAT_ALIGN_JUSTIFY);
    COUNT_EQUAL(2, FORMAT_ALIGN_CENTER);

    FR_ADD_STR(fr, 0, 100);
    fr.lastString()->setEqualCenter(true);
    fr.lastString()->setEqualRight(true);
    COUNT_EQUAL(2, FORMAT_ALIGN_LEFT);
    COUNT_EQUAL(3, FORMAT_ALIGN_RIGHT);
    COUNT_EQUAL(2, FORMAT_ALIGN_JUSTIFY);
    COUNT_EQUAL(2, FORMAT_ALIGN_CENTER);
}
