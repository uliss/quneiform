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

#include <fstream>

#include "testcedframe.h"
#include "../test_common.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestCEDFrame);
#include "common/tostring.h"
#include "ced/cedframe.h"
#include "ced/cedpicture.h"
#include "ced/cedparagraph.h"
#include "ced/cedline.h"
#include "ced/cedchar.h"

using namespace cf;

void TestCEDFrame::testSerialize() {
#ifdef CF_SERIALIZE
    const char * FILENAME = "serialize_cedframe.txt";
    CEDFrame fr;
    fr.setBorderSpace(11);
    writeToTextArchive(FILENAME, fr);

    CEDFrame new_fr;
    readFromTextArchive(FILENAME, fr);
#endif
}

void TestCEDFrame::testSerializeXml() {
#ifdef CF_SERIALIZE
    const char * FILENAME = "serialize_cedframe.xml";
    CEDFrame fr;
    fr.setBorderSpace(11);
    writeToXmlArchive(FILENAME, "cedframe", fr);

    CEDFrame new_fr;
    readFromXmlArchive(FILENAME, "cedframe", fr);
#endif
}

void TestCEDFrame::testClone()
{
    CEDFrame frame;
    frame.setBorderSpace(123);
    frame.setHPosition(CEDFrame::HPOS_PAGE);
    frame.setVPosition(CEDFrame::VPOS_PAGE);
    // block properties
    frame.setBorderBottom(ED_BRDR_DOTTED);
    frame.setBorderBottomWidth(100);
    frame.setBorderTop(ED_BRDR_DOTTED);
    frame.setBorderTopWidth(100);
    frame.setBorderLeft(ED_BRDR_DOTTED);
    frame.setBorderLeftWidth(100);
    frame.setBorderRight(ED_BRDR_DOTTED);
    frame.setBorderRightWidth(100);
    frame.setMargins(1, 2, 3, 4);

    CEDChar * ch_t = new CEDChar('t');
    ch_t->setColor(Color(0, 100, 200));
    CEDChar * ch_e = new CEDChar('e');
    CEDChar * ch_s = new CEDChar('s');
    frame.addElement(ch_t);
    frame.addElement(ch_e);
    frame.addElement(ch_s);
    frame.addElement(ch_t->clone());

    CEDFrame * frame_copy = frame.clone();
    CPPUNIT_ASSERT(frame_copy);
    CPPUNIT_ASSERT_EQUAL(frame.marginTop(), frame_copy->marginTop());
    CPPUNIT_ASSERT_EQUAL(frame.marginRight(), frame_copy->marginRight());
    CPPUNIT_ASSERT_EQUAL(frame.marginBottom(), frame_copy->marginBottom());
    CPPUNIT_ASSERT_EQUAL(frame.marginLeft(), frame_copy->marginLeft());
    CPPUNIT_ASSERT_EQUAL(frame.elementCount(), frame_copy->elementCount());
    CPPUNIT_ASSERT(frame.elementAt(0) != frame_copy->elementAt(0));
    CPPUNIT_ASSERT_EQUAL(frame.elementAt(0)->color(), frame_copy->elementAt(0)->color());
    CPPUNIT_ASSERT_EQUAL(static_cast<CEDChar*>(frame.elementAt(0))->alternativeAt(0),
                         static_cast<CEDChar*>(frame_copy->elementAt(0))->alternativeAt(0));
    CPPUNIT_ASSERT_EQUAL(static_cast<CEDChar*>(frame.elementAt(1))->alternativeAt(0),
                         static_cast<CEDChar*>(frame_copy->elementAt(1))->alternativeAt(0));
    CPPUNIT_ASSERT_EQUAL(static_cast<CEDChar*>(frame.elementAt(2))->alternativeAt(0),
                         static_cast<CEDChar*>(frame_copy->elementAt(2))->alternativeAt(0));
    CPPUNIT_ASSERT_EQUAL(static_cast<CEDChar*>(frame.elementAt(3))->alternativeAt(0),
                         static_cast<CEDChar*>(frame_copy->elementAt(3))->alternativeAt(0));
    delete frame_copy;
}
