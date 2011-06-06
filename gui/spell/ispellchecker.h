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

#ifndef ISPELLCHECKER_H
#define ISPELLCHECKER_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QSharedPointer>
#include "range.h"
#include "language.h"

class QTextDocument;

class ISpellChecker {
public:
    ISpellChecker(QTextDocument * doc);
    typedef QList<Range> SpellList;

    QTextDocument * document();
    const QTextDocument * document() const;
    virtual bool hasErrors(const QString& text) = 0;
    Language language() const;
    virtual bool setLanguage(const Language& lang);
    virtual SpellList spellErrors(const QString& text) = 0;
    virtual QStringList suggest(const QString& word) = 0;
    virtual QList<Language> supportedLanguages() const = 0;
private:
    QTextDocument * doc_;
    Language lang_;
};

typedef QSharedPointer<ISpellChecker> SpellCheckerPtr;

#endif // ISPELLCHECKER_H
