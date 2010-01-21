/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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
#include "testalphabetfactory.h"
#include <alphabets/alphabetfactory.h>
#include <alphabets/digits.h>
#include <alphabets/russian.h>
#include <list>
CPPUNIT_TEST_SUITE_REGISTRATION(TestAlphabetFactory);

using namespace CIF;
using namespace std;

void TestAlphabetFactory::testInit()
{
    AlphabetFactory * a1, *a2;
    a1 = &AlphabetFactory::instance();
    a2 = &AlphabetFactory::instance();

    CPPUNIT_ASSERT_EQUAL(a1, a2);

    language_t langs[] = {
        LANGUAGE_ENGLISH,
        LANGUAGE_GERMAN,
        LANGUAGE_FRENCH,
        LANGUAGE_RUSSIAN,
        LANGUAGE_SWEDISH,
        LANGUAGE_SPANISH,
        LANGUAGE_ITALIAN,
        LANGUAGE_RUS_ENG,
        LANGUAGE_UKRAINIAN,
        LANGUAGE_SERBIAN,
        LANGUAGE_CROATIAN,
        LANGUAGE_POLISH,
        LANGUAGE_DANISH,
        LANGUAGE_PORTUGUESE,
        LANGUAGE_DUTCH,
        LANGUAGE_DIGITS,
        //        LANGUAGE_UZBEK,
        //        LANGUAGE_KAZAKH,
        //        LANGUAGE_KAZ_ENG,
        LANGUAGE_CZECH,
        LANGUAGE_ROMANIAN,
        LANGUAGE_HUNGARIAN,
        LANGUAGE_BULGARIAN,
        LANGUAGE_SLOVENIAN,
        LANGUAGE_LATVIAN,
        LANGUAGE_LITHUANIAN,
        LANGUAGE_ESTONIAN,
        LANGUAGE_TURKISH };

    std::list<AlphabetPtr> alph;

    for (unsigned int i = 0; i < sizeof(langs) / sizeof(langs[0]); i++) {
        AlphabetPtr a = AlphabetFactory::instance().make(langs[i]);
        CPPUNIT_ASSERT_EQUAL(a->language(), langs[i]);
        alph.push_back(a);
    }

    CPPUNIT_ASSERT_THROW(AlphabetFactory::instance().alphabetTables(LANG_TOTAL), AlphabetException);

    for (std::list<AlphabetPtr>::iterator it = alph.begin(); it != alph.end(); ++it) {
        //        cerr << dec << (*it)->language() << endl;
        CPPUNIT_ASSERT(AlphabetFactory::instance().isLanguageData((*it)->language()));
    }
}

void TestAlphabetFactory::testSupportedLanguages()
{
    LanguageList lst = AlphabetFactory::instance().supportedLanguages();
    CPPUNIT_ASSERT(lst.size() > 0);
}

