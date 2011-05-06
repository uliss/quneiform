/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <QCoreApplication>
#include <QMap>
#include <QtAlgorithms>
#include <QDataStream>

#include "common/lang_def.h"
#include "alphabets/alphabetfactory.h"
#include "language.h"

typedef QMap<int, QString> LangCodeMap;
static LangCodeMap lang_code_map;
bool initLanguageCodeMap();

namespace {
    bool value = initLanguageCodeMap();
}

bool initLanguageCodeMap() {
    lang_code_map.insert(::LANGUAGE_BULGARIAN, QT_TRANSLATE_NOOP("Lang", "Bulgarian"));
    lang_code_map.insert(::LANGUAGE_CROATIAN, QT_TRANSLATE_NOOP("Lang", "Croatian"));
    lang_code_map.insert(::LANGUAGE_CZECH, QT_TRANSLATE_NOOP("Lang", "Czech"));
    lang_code_map.insert(::LANGUAGE_DANISH, QT_TRANSLATE_NOOP("Lang", "Danish"));
//    lang_code_map.insert(::LANGUAGE_DIGITS, QT_TRANSLATE_NOOP("Lang", "Digits"));
    lang_code_map.insert(::LANGUAGE_DUTCH, QT_TRANSLATE_NOOP("Lang", "Dutch"));
    lang_code_map.insert(::LANGUAGE_ENGLISH, QT_TRANSLATE_NOOP("Lang", "English"));
    lang_code_map.insert(::LANGUAGE_ESTONIAN, QT_TRANSLATE_NOOP("Lang", "Estonian"));
    lang_code_map.insert(::LANGUAGE_FRENCH, QT_TRANSLATE_NOOP("Lang", "French"));
    lang_code_map.insert(::LANGUAGE_GERMAN, QT_TRANSLATE_NOOP("Lang", "German"));
    lang_code_map.insert(::LANGUAGE_HUNGARIAN, QT_TRANSLATE_NOOP("Lang", "Hungarian"));
    lang_code_map.insert(::LANGUAGE_ITALIAN, QT_TRANSLATE_NOOP("Lang", "Italian"));
//    lang_code_map.insert(::LANGUAGE_KAZAKH, QT_TRANSLATE_NOOP("Lang", "Kazakh"));
//    lang_code_map.insert(::LANGUAGE_KAZ_ENG, "Kazakh-English");
    lang_code_map.insert(::LANGUAGE_LATVIAN, QT_TRANSLATE_NOOP("Lang", "Latvian"));
    lang_code_map.insert(::LANGUAGE_LITHUANIAN, QT_TRANSLATE_NOOP("Lang", "Lithuanian"));
    lang_code_map.insert(::LANGUAGE_POLISH, QT_TRANSLATE_NOOP("Lang", "Polish"));
    lang_code_map.insert(::LANGUAGE_PORTUGUESE, QT_TRANSLATE_NOOP("Lang", "Portuguese"));
    lang_code_map.insert(::LANGUAGE_ROMANIAN, QT_TRANSLATE_NOOP("Lang", "Romanian"));
    lang_code_map.insert(::LANGUAGE_RUSSIAN, QT_TRANSLATE_NOOP("Lang", "Russian"));
    lang_code_map.insert(::LANGUAGE_RUS_ENG, QT_TRANSLATE_NOOP("Lang", "Russian-English"));
    lang_code_map.insert(::LANGUAGE_SERBIAN, QT_TRANSLATE_NOOP("Lang", "Serbian"));
    lang_code_map.insert(::LANGUAGE_SLOVENIAN, QT_TRANSLATE_NOOP("Lang", "Slovenian"));
    lang_code_map.insert(::LANGUAGE_SPANISH, QT_TRANSLATE_NOOP("Lang", "Spanish"));
    lang_code_map.insert(::LANGUAGE_SWEDISH, QT_TRANSLATE_NOOP("Lang", "Swedish"));
    lang_code_map.insert(::LANGUAGE_TURKISH, QT_TRANSLATE_NOOP("Lang", "Turkish"));
    lang_code_map.insert(::LANGUAGE_UKRAINIAN, QT_TRANSLATE_NOOP("Lang", "Ukrainian"));
//    lang_code_map.insert(::LANGUAGE_UZBEK, QT_TRANSLATE_NOOP("Lang", "Uzbek");
    return true;
}

bool compareLanguageByName(const Language& l1, const Language& l2) {
    return l1.name() < l2.name();
}

bool compareLanguageByTrName(const Language& l1, const Language& l2) {
    return l1.trName() < l2.trName();
}

Language::Language(int code)
{
    LangCodeMap::iterator it = lang_code_map.find(code);
    if(it != lang_code_map.end()) {
        code_ = it.key();
        name_ = it.value();
    }
    else {
        code_ = ::LANGUAGE_UNKNOWN;
    }
}

int Language::code() const {
    return code_;
}

bool Language::isValid() const {
    return code_ >= 0;
}

QString Language::name() const {
    return name_;
}

QList<Language> Language::supportedLanguages(Sorting sort) {
    QList<Language> ret;

    cf::LanguageList langs = cf::AlphabetFactory::instance().supportedLanguages();
    for (cf::LanguageList::iterator it = langs.begin(), end = langs.end(); it != end; ++it) {
        Language language(*it);

        if(language.isValid())
            ret.append(language);
    }

    switch(sort) {
    case BY_NAME:
        qSort(ret.begin(), ret.end(), compareLanguageByName);
        break;
    case BY_TR_NAME:
        qSort(ret.begin(), ret.end(), compareLanguageByName);
        break;
    case NO_SORT:
    default:
        break;
    }

    return ret;
}

QString Language::trName() const {
    return QCoreApplication::translate("Lang", name_.toAscii().constData());
}

QDataStream& operator<<(QDataStream& stream, const Language& l) {
    stream << l.code_;
    return stream;
}

QDataStream& operator>>(QDataStream& stream, Language& l) {
    int code = -1;
    stream >> code;
    l = Language(code);
    return stream;
}
