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

#include <QApplication>

#include "language_i18n.h"
#include "common/language.h"
#include "alphabets/alphabetfactory.h"

namespace {
    static const char * lang_names[]  = {
        QT_TRANSLATE_NOOP("Lang", "Bulgarian"),
        QT_TRANSLATE_NOOP("Lang", "Croatian"),
        QT_TRANSLATE_NOOP("Lang", "Czech"),
        QT_TRANSLATE_NOOP("Lang", "Danish"),
        QT_TRANSLATE_NOOP("Lang", "Digits"),
        QT_TRANSLATE_NOOP("Lang", "Dutch"),
        QT_TRANSLATE_NOOP("Lang", "English"),
        QT_TRANSLATE_NOOP("Lang", "Estonian"),
        QT_TRANSLATE_NOOP("Lang", "French"),
        QT_TRANSLATE_NOOP("Lang", "German"),
        QT_TRANSLATE_NOOP("Lang", "Hungarian"),
        QT_TRANSLATE_NOOP("Lang", "Italian"),
        QT_TRANSLATE_NOOP("Lang", "Latvian"),
        QT_TRANSLATE_NOOP("Lang", "Lithuanian"),
        QT_TRANSLATE_NOOP("Lang", "Polish"),
        QT_TRANSLATE_NOOP("Lang", "Portuguese"),
        QT_TRANSLATE_NOOP("Lang", "Romanian"),
        QT_TRANSLATE_NOOP("Lang", "Russian"),
        QT_TRANSLATE_NOOP("Lang", "Russian-English"),
        QT_TRANSLATE_NOOP("Lang", "Serbian"),
        QT_TRANSLATE_NOOP("Lang", "Slovak"),
        QT_TRANSLATE_NOOP("Lang", "Spanish"),
        QT_TRANSLATE_NOOP("Lang", "Swedish"),
        QT_TRANSLATE_NOOP("Lang", "Turkish"),
        QT_TRANSLATE_NOOP("Lang", "Ukrainian")
    };
}

LanguageMap supportedLanguages() {
    LanguageMap lmap;
    using namespace cf;
    LanguageList langs = AlphabetFactory::instance().supportedLanguages();
    for (LanguageList::iterator it = langs.begin(), end = langs.end();
                    it != end;
                    ++it) {
        // skipping digits
        if(*it == LANGUAGE_DIGITS)
            continue;

        lmap.insert(QApplication::translate("Lang", Language::isoName(*it).c_str()), *it);
    }
    return lmap;
}

