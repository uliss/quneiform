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

#include <QDebug>

#include "macspellchecker.h"
#include "macspellchecker_private.h"
#include "common/language.h"

MacSpellChecker::MacSpellChecker(const Language& lang)
    : ISpellChecker(lang)
{
    spellInit();
    MacSpellChecker::setLanguage(lang);
}

int64_t MacSpellChecker::documentTag() const{
    return language().code();
}

bool MacSpellChecker::hasErrors(const QString& text) {
    return !spellErrors(text).isEmpty();
}

bool MacSpellChecker::setLanguage(const Language& lang) {
    if(!supportedLanguages().contains(lang)) {
        qDebug() << Q_FUNC_INFO << QString("language '%1' is not supporterd").arg(lang.name());
        return false;
    }

    bool result = ::setSpellLanguage(lang);
    ISpellChecker::setLanguage(lang);
    return result;
}

ISpellChecker::SpellList MacSpellChecker::spellErrors(const QString& text) {
    return ::checkSpelling(text, documentTag());
}

QStringList MacSpellChecker::suggest(const QString& word) {
    return ::suggest(word, documentTag());
}

QList<Language> MacSpellChecker::supportedLanguages() const {
    return ::supportedLanguages();
}
