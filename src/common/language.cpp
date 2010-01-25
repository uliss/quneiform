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

namespace CIF
{

static LanguageMap lang_map;

static inline void addCodeName(language_t language, const std::string& code,
        const std::string& name)
{
    lang_map[language] = LanguageCodeName(code, name);
}

namespace
{

bool languageNamesInit()
{
    addCodeName(LANGUAGE_BULGARIAN, "bul", "Bulgarian");
    addCodeName(LANGUAGE_CROATIAN, "hrv", "Croatian");
    addCodeName(LANGUAGE_CZECH, "cze", "Czech");
    addCodeName(LANGUAGE_DANISH, "dan", "Danish");
    addCodeName(LANGUAGE_DIGITS, "dig", "Digits");
    addCodeName(LANGUAGE_DUTCH, "dut", "Dutch");
    addCodeName(LANGUAGE_ENGLISH, "eng", "English");
    addCodeName(LANGUAGE_ESTONIAN, "est", "Estonian");
    addCodeName(LANGUAGE_FRENCH, "fra", "French");
    addCodeName(LANGUAGE_GERMAN, "ger", "German");
    addCodeName(LANGUAGE_HUNGARIAN, "hun", "Hungarian");
    addCodeName(LANGUAGE_ITALIAN, "ita", "Italian");
    addCodeName(LANGUAGE_KAZAKH, "kaz", "Kazakh");
    addCodeName(LANGUAGE_KAZ_ENG, "kazeng", "Kazakh-English");
    addCodeName(LANGUAGE_LATVIAN, "lav", "Latvian");
    addCodeName(LANGUAGE_LITHUANIAN, "lit", "Lithuanian");
    addCodeName(LANGUAGE_POLISH, "pol", "Polish");
    addCodeName(LANGUAGE_PORTUGUESE, "por", "Portuguese");
    addCodeName(LANGUAGE_ROMANIAN, "rum", "Romanian");
    addCodeName(LANGUAGE_RUSSIAN, "rus", "Russian");
    addCodeName(LANGUAGE_RUS_ENG, "ruseng", "Russian-English");
    addCodeName(LANGUAGE_SERBIAN, "srp", "Serbian");
    addCodeName(LANGUAGE_SLOVENIAN, "slo", "Slovenian");
    addCodeName(LANGUAGE_SPANISH, "spa", "Spanish");
    addCodeName(LANGUAGE_SWEDISH, "swe", "Swedish");
    addCodeName(LANGUAGE_TURKISH, "tur", "Turkish");
    addCodeName(LANGUAGE_UKRAINIAN, "ukr", "Ukrainian");
    addCodeName(LANGUAGE_UZBEK, "uzb", "Uzbek");
    return true;
}

const bool init = languageNamesInit();
}

Language::Language(language_t language) :
    language_(language)
{
}

std::string Language::isoCode(language_t language)
{
    LanguageMap::iterator it = lang_map.find(language);
    return it != lang_map.end() ? it->second.first : "???";
}

std::string Language::isoCode() const
{
    return isoCode(language_);
}

std::string Language::isoName() const
{
    return isoName(language_);
}

bool Language::isValid() const
{
	return (language_ < 0 || language_ >= LANG_TOTAL) ? false : true;
}

std::string Language::isoName(language_t language)
{
    LanguageMap::iterator it = lang_map.find(language);
    return it != lang_map.end() ? it->second.second : "Unknown";
}

Language Language::byCode(const std::string& code)
{
    for (LanguageMap::iterator it = lang_map.begin(), end = lang_map.end(); it != end; ++it) {
        if (it->second.first == code)
            return Language(it->first);
    }
    return Language(LANGUAGE_UNKNOWN);
}

Language Language::byName(const std::string& name)
{
	for (LanguageMap::iterator it = lang_map.begin(), end = lang_map.end(); it != end; ++it) {
        if (it->second.second == name)
            return Language(it->first);
    }
    return Language(LANGUAGE_UNKNOWN);
}

LanguageList Language::languages(sort_t sort_mode)
{
	LanguageList ret = languagesAll();
    switch(sort_mode) {
    case SORT_BY_CODE:
    	sortByCode(ret);
    case SORT_BY_NAME:
    	sortByName(ret);
    default:
    	break;
    }
    return ret;
}

LanguageList Language::languagesAll()
{
	LanguageList ret;
	for (LanguageMap::iterator it = lang_map.begin(), end = lang_map.end(); it != end; ++it)
		ret.push_back(it->first);
	return ret;
}

inline bool cmpByCode(language_t one, language_t two)
{
	return lang_map[one].first.compare(lang_map[two].first) < 0;
}

void Language::sortByCode(LanguageList& lst)
{
	lst.sort(cmpByCode);
}

inline bool cmpByName(language_t one, language_t two)
{
	return lang_map[one].second.compare(lang_map[two].second) < 0;
}

void Language::sortByName(LanguageList& lst)
{
	lst.sort(cmpByName);
}

std::ostream& operator<<(std::ostream& os, const Language& language)
{
    os << language.isoName();
    return os;
}

}
