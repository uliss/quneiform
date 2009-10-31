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

void TestAlphabetFactory::testInit() {
    AlphabetFactory * a1, *a2;
    a1 = &AlphabetFactory::instance();
    a2 = &AlphabetFactory::instance();

    CPPUNIT_ASSERT_EQUAL(a1, a2);

    language_t langs[] = {
        LANG_ENGLISH,
        LANG_GERMAN,
        LANG_FRENCH,
        LANG_RUSSIAN,
        LANG_SWEDISH,
        LANG_SPANISH,
        LANG_ITALIAN,
        LANG_RUSENG,
        LANG_UKRAINIAN,
        LANG_SERBIAN,
        LANG_CROATIAN,
        LANG_POLISH,
        LANG_DANISH,
        LANG_PORTUGUESE,
        LANG_DUTCH,
        LANG_DIG,
        LANG_UZBEK,
        LANG_KAZ,
        LANG_KAZ_ENG,
        LANG_CZECH,
        LANG_ROMAN,
        LANG_HUNGAR,
        LANG_BULGAR,
        LANG_SLOVENIAN,
        LANG_LATVIAN,
        LANG_LITHUANIAN,
        LANG_ESTONIAN,
        LANG_TURKISH };

    std::list<Alphabet*> alph;

    for (unsigned int i = 0; i < sizeof(langs) / sizeof(langs[0]); i++) {
        Alphabet * a = AlphabetFactory::instance().make(langs[i]);
        CPPUNIT_ASSERT_EQUAL(a->language(), langs[i]);
        alph.push_back(a);
    }

    for(std::list<Alphabet*>::iterator it = alph.begin(); it != alph.end(); ++it)
        delete *it;
}

