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

#include "language.h"

namespace cf
{

struct LanguageData
{
        LanguageData() {}
        LanguageData(const std::string& isoCode2, const std::string& isoCode3,
                const std::string& isoName) :
            code2(isoCode2), code3(isoCode3), name(isoName) {
        }

        std::string code2;
        std::string code3;
        std::string name;
};

typedef std::map<language_t, LanguageData> LanguageMap;

static LanguageMap lang_map;

static inline void addCodeName(language_t language, const std::string& code2,
        const std::string& code3, const std::string& name) {
    LanguageData lang_data(code2, code3, name);
    lang_map[language] = lang_data;
}

namespace
{

// sets language data according to ISO 639-1 and 639-2
bool languageNamesInit() {
    addCodeName(LANGUAGE_BULGARIAN,   "bg", "bul", "Bulgarian");
    addCodeName(LANGUAGE_BELARUSIAN,  "by", "bel", "Belarusian");
    addCodeName(LANGUAGE_CROATIAN,    "hr", "hrv", "Croatian");
    addCodeName(LANGUAGE_CZECH,       "cs", "cze", "Czech");
    addCodeName(LANGUAGE_DANISH,      "da", "dan", "Danish");
    addCodeName(LANGUAGE_DIGITS,      "di", "dig", "Digits");
    addCodeName(LANGUAGE_DUTCH,       "nl", "dut", "Dutch");
    addCodeName(LANGUAGE_ENGLISH,     "en", "eng", "English");
    addCodeName(LANGUAGE_ESTONIAN,    "et", "est", "Estonian");
    addCodeName(LANGUAGE_FRENCH,      "fr", "fra", "French");
    addCodeName(LANGUAGE_GERMAN,      "de", "ger", "German");
    addCodeName(LANGUAGE_HUNGARIAN,   "hu", "hun", "Hungarian");
    addCodeName(LANGUAGE_ITALIAN,     "it", "ita", "Italian");
    addCodeName(LANGUAGE_KAZAKH,      "kk", "kaz", "Kazakh");
    addCodeName(LANGUAGE_KAZ_ENG,     "kken", "kazeng", "Kazakh-English");
    addCodeName(LANGUAGE_LATVIAN,     "lv", "lav", "Latvian");
    addCodeName(LANGUAGE_LITHUANIAN,  "lt", "lit", "Lithuanian");
    addCodeName(LANGUAGE_POLISH,      "pl", "pol", "Polish");
    addCodeName(LANGUAGE_PORTUGUESE,  "pt", "por", "Portuguese");
    addCodeName(LANGUAGE_ROMANIAN,    "ro", "rum", "Romanian");
    addCodeName(LANGUAGE_RUSSIAN,     "ru", "rus", "Russian");
    addCodeName(LANGUAGE_RUS_ENG,     "ruen", "ruseng", "Russian-English");
    addCodeName(LANGUAGE_SERBIAN,     "sr", "srp", "Serbian");
    addCodeName(LANGUAGE_SLOVENIAN,   "sl", "slv", "Slovenian");
    addCodeName(LANGUAGE_SPANISH,     "es", "spa", "Spanish");
    addCodeName(LANGUAGE_SWEDISH,     "sw", "swe", "Swedish");
    addCodeName(LANGUAGE_TURKISH,     "tr", "tur", "Turkish");
    addCodeName(LANGUAGE_UKRAINIAN,   "uk", "ukr", "Ukrainian");
    addCodeName(LANGUAGE_UZBEK,       "uz", "uzb", "Uzbek");
    return true;
}

const bool init = languageNamesInit();
}

Language::Language(language_t language) :
    language_(language) {
}

std::string Language::encoding() const {
    return encoding(*this);
}

std::string Language::encoding(const Language& l) {
    switch (l.language_) {
    case LANGUAGE_CROATIAN:
    case LANGUAGE_CZECH:
    case LANGUAGE_HUNGARIAN:
    case LANGUAGE_POLISH:
    case LANGUAGE_ROMANIAN:
    case LANGUAGE_SLOVENIAN:
        return "cp1250";
        // for serbian cyrillic
    case LANGUAGE_SERBIAN:
    case LANGUAGE_BULGARIAN:
    case LANGUAGE_KAZAKH:
    case LANGUAGE_KAZ_ENG:
    case LANGUAGE_RUSSIAN:
    case LANGUAGE_RUS_ENG:
    case LANGUAGE_UKRAINIAN:
    case LANGUAGE_UZBEK:
    case LANGUAGE_BELARUSIAN:
        return "cp1251";
    case LANGUAGE_DANISH:
    case LANGUAGE_DUTCH:
    case LANGUAGE_ENGLISH:
    case LANGUAGE_FRENCH:
    case LANGUAGE_GERMAN:
    case LANGUAGE_ITALIAN:
    case LANGUAGE_PORTUGUESE:
    case LANGUAGE_SPANISH:
    case LANGUAGE_SWEDISH:
        return "cp1252";
    case LANGUAGE_TURKISH:
        return "cp1254";
    case LANGUAGE_ESTONIAN:
    case LANGUAGE_LATVIAN:
    case LANGUAGE_LITHUANIAN:
        return "cp1257";
    default:
        return "latin1";
    }
}

std::string Language::isoCode2() const {
    return isoCode2(language_);
}

std::string Language::isoCode2(language_t language) {
    LanguageMap::iterator it = lang_map.find(language);
    return it != lang_map.end() ? it->second.code2 : std::string();
}

std::string Language::isoCode3() const {
    return isoCode3(language_);
}

std::string Language::isoCode3(language_t language) {
    LanguageMap::iterator it = lang_map.find(language);
    return it != lang_map.end() ? it->second.code3 : std::string();
}

std::string Language::isoName() const {
    return isoName(language_);
}

bool Language::isValid() const {
    return (language_ < 0 || language_ >= LANG_TOTAL) ? false : true;
}

std::string Language::isoName(language_t language) {
    LanguageMap::iterator it = lang_map.find(language);
    return it != lang_map.end() ? it->second.name : "";
}

Language Language::byCode2(const std::string& code) {
    for (LanguageMap::iterator it = lang_map.begin(), end = lang_map.end(); it != end; ++it) {
        if (it->second.code2 == code)
            return Language(it->first);
    }
    return Language(LANGUAGE_UNKNOWN);
}

Language Language::byCode3(const std::string& code) {
    for (LanguageMap::iterator it = lang_map.begin(), end = lang_map.end(); it != end; ++it) {
        if (it->second.code3 == code)
            return Language(it->first);
    }
    return Language(LANGUAGE_UNKNOWN);
}

Language Language::byName(const std::string& name) {
    for (LanguageMap::iterator it = lang_map.begin(), end = lang_map.end(); it != end; ++it) {
        if (it->second.name == name)
            return Language(it->first);
    }
    return Language(LANGUAGE_UNKNOWN);
}

LanguageList Language::allLanguages(sort_t sort_mode) {
    LanguageList ret = languagesAll();
    switch (sort_mode) {
    case SORT_BY_CODE2:
        sortByCode2(ret);
        break;
    case SORT_BY_CODE3:
        sortByCode3(ret);
        break;
    case SORT_BY_NAME:
        sortByName(ret);
        break;
    default:
        break;
    }
    return ret;
}

LanguageList Language::languagesAll() {
    LanguageList ret;
    for (LanguageMap::iterator it = lang_map.begin(), end = lang_map.end(); it != end; ++it)
        ret.push_back(it->first);
    return ret;
}

inline bool cmpByCode2(language_t one, language_t two) {
    return lang_map[one].code2.compare(lang_map[two].code2) < 0;
}

inline bool cmpByCode3(language_t one, language_t two) {
    return lang_map[one].code3.compare(lang_map[two].code3) < 0;
}

void Language::sortByCode2(LanguageList& lst) {
    lst.sort(cmpByCode2);
}

void Language::sortByCode3(LanguageList& lst) {
    lst.sort(cmpByCode3);
}

inline bool cmpByName(language_t one, language_t two) {
    return lang_map[one].name.compare(lang_map[two].name) < 0;
}

void Language::sortByName(LanguageList& lst) {
    lst.sort(cmpByName);
}

std::ostream& operator<<(std::ostream& os, const Language& language) {
    os << language.isoName();
    return os;
}

}
