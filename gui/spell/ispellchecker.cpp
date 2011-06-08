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

#include "ispellchecker.h"

ISpellChecker::ISpellChecker(const Language& lang)
    : lang_(lang),
    suggest_limit_(5)
{}

bool ISpellChecker::isSupported(const Language& lang) {
    return supportedLanguages().contains(lang);
}

Language ISpellChecker::language() const {
    return lang_;
}

bool ISpellChecker::setLanguage(const Language& lang) {
    lang_ = lang;
    return true;
}

void ISpellChecker::setSuggestLimit(uint limit) {
    suggest_limit_ = limit;
}

uint ISpellChecker::suggestLimit() const {
    return suggest_limit_;
}
