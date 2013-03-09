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

#include <sstream>

#include "testlanguage.h"
#include "common/language.h"
#include "common/tostring.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestLanguage);

using namespace cf;

void TestLanguage::testInit() {
    Language l(LANGUAGE_RUSSIAN);
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_RUSSIAN, l.get());

    CPPUNIT_ASSERT_EQUAL(size_t(29), Language::allLanguages().size());
}

void TestLanguage::testIsoNames() {
    static language_t langs[] = { LANGUAGE_ENGLISH, LANGUAGE_GERMAN, LANGUAGE_FRENCH,
            LANGUAGE_RUSSIAN, LANGUAGE_SWEDISH, LANGUAGE_SPANISH, LANGUAGE_ITALIAN,
            LANGUAGE_RUS_ENG, LANGUAGE_UKRAINIAN, LANGUAGE_SERBIAN, LANGUAGE_CROATIAN,
            LANGUAGE_POLISH, LANGUAGE_DANISH, LANGUAGE_PORTUGUESE, LANGUAGE_DUTCH, LANGUAGE_DIGITS,
            LANGUAGE_UZBEK, LANGUAGE_KAZAKH, LANGUAGE_KAZ_ENG, LANGUAGE_CZECH, LANGUAGE_ROMANIAN,
            LANGUAGE_HUNGARIAN, LANGUAGE_BULGARIAN, LANGUAGE_SLOVENIAN, LANGUAGE_LATVIAN,
            LANGUAGE_LITHUANIAN, LANGUAGE_ESTONIAN, LANGUAGE_TURKISH };

    static const int num_langs = sizeof(langs) / sizeof(langs[0]);

    static const char * iso_names[] = { "English", "German", "French", "Russian", "Swedish",
            "Spanish", "Italian", "Russian-English", "Ukrainian", "Serbian", "Croatian", "Polish",
            "Danish", "Portuguese", "Dutch", "Digits", "Uzbek", "Kazakh", "Kazakh-English",
            "Czech", "Romanian", "Hungarian", "Bulgarian", "Slovak", "Latvian", "Lithuanian",
            "Estonian", "Turkish" };

    static const int num_names = sizeof(iso_names) / sizeof(iso_names[0]);

    static const char * iso_codes3[] = { "eng", "ger", "fra", "rus", "swe", "spa", "ita", "ruseng",
            "ukr", "srp", "hrv", "pol", "dan", "por", "dut", "dig", "uzb", "kaz", "kazeng", "cze",
            "rum", "hun", "bul", "slo", "lav", "lit", "est", "tur" };

    static const char * iso_codes2[] = { "en", "de", "fr", "ru", "sw", "es", "it", "ruen", "uk",
            "sr", "hr", "pl", "da", "pt", "nl", "di", "uz", "kk", "kken", "cs", "ro", "hu", "bg",
            "sk", "lv", "lt", "et", "tr" };

    static const int num_codes = sizeof(iso_codes3) / sizeof(iso_codes3[0]);

    CPPUNIT_ASSERT(num_langs == num_names && num_langs == num_codes);
    for (int i = 0; i < num_langs; i++) {
        Language lang(langs[i]);
        CPPUNIT_ASSERT_EQUAL(std::string(iso_codes2[i]), lang.isoCode2());
        CPPUNIT_ASSERT_EQUAL(std::string(iso_codes3[i]), lang.isoCode3());
        CPPUNIT_ASSERT_EQUAL(std::string(iso_names[i]), lang.isoName());
    }

    Language l(LANG_TOTAL);
    CPPUNIT_ASSERT(l.isoCode3() == "");
    CPPUNIT_ASSERT(l.isoName() == "");
}

void TestLanguage::testNamesSort() {
    LanguageList lst;

    lst.push_back(LANGUAGE_CROATIAN);
    lst.push_back(LANGUAGE_DUTCH);
    lst.push_back(LANGUAGE_RUSSIAN);
    lst.push_back(LANGUAGE_UKRAINIAN);

    Language::sortByName(lst);
    LanguageList::iterator it = lst.begin();
    CPPUNIT_ASSERT_EQUAL(*it, LANGUAGE_CROATIAN);
    it++;
    CPPUNIT_ASSERT_EQUAL(*it, LANGUAGE_DUTCH);
    it++;
    CPPUNIT_ASSERT_EQUAL(*it, LANGUAGE_RUSSIAN);
    it++;
    CPPUNIT_ASSERT_EQUAL(*it, LANGUAGE_UKRAINIAN);

    Language::sortByCode3(lst);
    it = lst.begin();
    CPPUNIT_ASSERT_EQUAL(*it, LANGUAGE_DUTCH);
    it++;
    CPPUNIT_ASSERT_EQUAL(*it, LANGUAGE_CROATIAN);
    it++;
    CPPUNIT_ASSERT_EQUAL(*it, LANGUAGE_RUSSIAN);
    it++;
    CPPUNIT_ASSERT_EQUAL(*it, LANGUAGE_UKRAINIAN);
}

void TestLanguage::testIsValid() {
    for (language_t lang = LANGUAGE_ENGLISH; lang < LANG_TOTAL; lang = (language_t) (lang + 1)) {
        Language l(lang);
        CPPUNIT_ASSERT(l.isValid());
    }

    Language l(LANGUAGE_UNKNOWN);
    CPPUNIT_ASSERT(!l.isValid());
    l = Language(LANG_TOTAL);
    CPPUNIT_ASSERT(!l.isValid());
}

void TestLanguage::testByCode() {
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_ENGLISH, Language::byCode3("eng").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_RUS_ENG, Language::byCode3("ruseng").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_UNKNOWN, Language::byCode3("?????").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_DIGITS, Language::byCode3("dig").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_GERMAN, Language::byCode3("ger").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_DUTCH, Language::byCode3("dut").get());

    CPPUNIT_ASSERT_EQUAL(LANGUAGE_ENGLISH, Language::byCode2("en").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_RUS_ENG, Language::byCode2("ruen").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_UNKNOWN, Language::byCode2("?????").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_DIGITS, Language::byCode2("di").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_GERMAN, Language::byCode2("de").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_DUTCH, Language::byCode2("nl").get());
}

void TestLanguage::testByName() {
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_ENGLISH, Language::byName("English").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_RUS_ENG, Language::byName("Russian-English").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_UNKNOWN, Language::byName("?????").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_DIGITS, Language::byName("Digits").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_GERMAN, Language::byName("German").get());
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_DUTCH, Language::byName("Dutch").get());
}

void TestLanguage::testOutput()
{
    std::ostringstream buf;
    buf << Language(LANGUAGE_ESTONIAN);
    CPPUNIT_ASSERT_EQUAL(std::string("Estonian"), buf.str());
}

void TestLanguage::testEncodings()
{
    Language l(LANGUAGE_RUSSIAN);
    CPPUNIT_ASSERT_EQUAL(std::string("cp1251"), l.encoding());

    l = Language(LANGUAGE_POLISH);
    CPPUNIT_ASSERT_EQUAL(std::string("cp1250"), l.encoding());

    l = Language(LANGUAGE_ENGLISH);
    CPPUNIT_ASSERT_EQUAL(std::string("cp1252"), l.encoding());

    l = Language(LANGUAGE_TURKISH);
    CPPUNIT_ASSERT_EQUAL(std::string("cp1254"), l.encoding());

    l = Language(LANGUAGE_ESTONIAN);
    CPPUNIT_ASSERT_EQUAL(std::string("cp1257"), l.encoding());

    l = Language(LANG_TOTAL);
    CPPUNIT_ASSERT_EQUAL(std::string("latin1"), l.encoding());
}

void TestLanguage::testAll()
{
    LanguageList lst = Language::allLanguages();
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_ENGLISH, lst.front());

    lst = Language::allLanguages(Language::SORT_BY_NAME);
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_BELARUSIAN, lst.front());

    lst = Language::allLanguages(Language::SORT_BY_CODE2);
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_BULGARIAN, lst.front());

    lst = Language::allLanguages(Language::SORT_BY_CODE3);
    CPPUNIT_ASSERT_EQUAL(LANGUAGE_BELARUSIAN, lst.front());
}
