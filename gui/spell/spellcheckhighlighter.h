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

#ifndef SPELLCHECKHIGHLIGHTER_H
#define SPELLCHECKHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include "ispellchecker.h"

class SpellCheckHighlighter : public QSyntaxHighlighter
{
public:
    SpellCheckHighlighter(QTextDocument * textDocument);
    SpellCheckHighlighter(QTextEdit * textEdit);
    void highlightBlock(const QString& text);
    bool isEnabled() const;
    Language language() const;
    void setEnabled(bool value = true);
    bool setLanguage(const Language& lang);
    SpellCheckerPtr spellChecker();
private:
    SpellCheckerPtr spell_checker_;
    Language lang_;
    bool enabled_;
};

#endif // SPELLCHECKHIGHLIGHTER_H
