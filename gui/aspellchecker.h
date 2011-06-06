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

#ifndef ASPELLCHECKER_H
#define ASPELLCHECKER_H

#include "ispellchecker.h"

class ASpellChecker : public ISpellChecker
{
public:
    ASpellChecker(QTextDocument * doc);
    bool hasErrors(const QString& text);
    SpellList spellErrors(const QString& text);
    QStringList suggest(const QString& word);
    QList<Language> supportedLanguages() const;
};

#endif // ASPELLCHECKER_H
