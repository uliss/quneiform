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
#include "testlanguage.h"
#include "common/language.h"
CPPUNIT_TEST_SUITE_REGISTRATION(TestLanguage);

using namespace CIF;

void TestLanguage::testInit()
{
    Language l(LANGUAGE_RUSSIAN);
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_RUSSIAN, l.get());
}

void TestLanguage::testIsoNames()
{
    static language_t langs[] = {
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
        LANGUAGE_UZBEK,
        LANGUAGE_KAZAKH,
        LANGUAGE_KAZ_ENG,
        LANGUAGE_CZECH,
        LANGUAGE_ROMANIAN,
        LANGUAGE_HUNGARIAN,
        LANGUAGE_BULGARIAN,
        LANGUAGE_SLOVENIAN,
        LANGUAGE_LATVIAN,
        LANGUAGE_LITHUANIAN,
        LANGUAGE_ESTONIAN,
        LANGUAGE_TURKISH };

    static const int num_langs = sizeof(langs) / sizeof(langs[0]);

    static const char * iso_names[] = {
        "English",
        "German",
        "French",
        "Russian",
        "Swedish",
        "Spanish",
        "Italian",
        "Russian-English",
        "Ukrainian",
        "Serbian",
        "Croatian",
        "Polish",
        "Danish",
        "Portuguese",
        "Dutch",
        "Digits",
        "Uzbek",
        "Kazakh",
        "Kazakh-English",
        "Czech",
        "Romanian",
        "Hungarian",
        "Bulgarian",
        "Slovenian",
        "Latvian",
        "Lithuanian",
        "Estonian",
        "Turkish" };

    static const int num_names = sizeof(iso_names) / sizeof(iso_names[0]);

    static const char * iso_codes[] = {
        "eng",
        "ger",
        "fra",
        "rus",
        "swe",
        "spa",
        "ita",
        "ruseng",
        "ukr",
        "srp",
        "hrv",
        "pol",
        "dan",
        "por",
        "dut",
        "dig",
        "uzb",
        "kaz",
        "kazeng",
        "cze",
        "rum",
        "hun",
        "bul",
        "slo",
        "lav",
        "lit",
        "est",
        "tur" };
    static const int num_codes = sizeof(iso_codes) / sizeof(iso_codes[0]);

    CPPUNIT_ASSERT(num_langs == num_names && num_langs == num_codes);
    for (int i = 0; i < num_langs; i++) {
        Language lang(langs[i]);
        CPPUNIT_ASSERT(lang.isoCode() == iso_codes[i]);
        CPPUNIT_ASSERT(lang.isoName() == iso_names[i]);
    }

    Language l(LANG_TOTAL);
    CPPUNIT_ASSERT(l.isoCode() == "???");
    CPPUNIT_ASSERT(l.isoName() == "Unknown");
}
