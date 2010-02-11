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
#include "testcomponentrecognizer.h"
#define private public
#include "rreccom/componentrecognizer.h"
#include "common/language.h"
#include "ccom/ccom.h"
#include "alphabets/alphabetfactory.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestComponentRecognizer);
using namespace CIF;

void TestComponentRecognizer::testInit() {
    ComponentRecognizer r;
    CPPUNIT_ASSERT_THROW(r.recognize(0, LANG_TOTAL), ComponentRecognizer::Exception);
    r.recognize(0, LANGUAGE_ENGLISH);
}

void TestComponentRecognizer::testAlphabetInit() {
    ComponentRecognizer r;
    LanguageList lst = AlphabetFactory::instance().supportedLanguages();
    for (LanguageList::iterator it = lst.begin(), end = lst.end(); it != end; ++it) {
        r.alphabetInit(*it);
        r.loadAlphabetTables(*it);
    }
    CPPUNIT_ASSERT_THROW(r.alphabetInit(LANGUAGE_UNKNOWN), ComponentRecognizer::Exception);
    CPPUNIT_ASSERT_THROW(r.loadAlphabetTables(LANGUAGE_UNKNOWN), AlphabetException);
}

void TestComponentRecognizer::testRecognizeComponents() {
    CCOM_Init(0);
    ComponentRecognizer r;
    CCOM_handle cont = CCOM_CreateContainer();
    r.recognizeComponents(cont);
    CCOM_DeleteContainer(cont);
    CCOM_Done();
}

void TestComponentRecognizer::testRecognizeComponent() {
    ComponentRecognizer r;
    CPPUNIT_ASSERT_THROW(r.recognizeComponent(NULL), ComponentRecognizer::Exception);
}
