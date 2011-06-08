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

#include <QTextCharFormat>
#include <QDebug>

#include "spellcheckhighlighter.h"
#include "spellcheckerfactory.h"

SpellCheckHighlighter::SpellCheckHighlighter(QTextDocument * textDocument)
       : QSyntaxHighlighter(textDocument),
         enabled_(true)
{
}

SpellCheckHighlighter::SpellCheckHighlighter(QTextEdit * textEdit)
     : QSyntaxHighlighter(textEdit),
       enabled_(true)
{
}

void SpellCheckHighlighter::highlightBlock(const QString& text)
{
    if (!enabled_)
        return;

    QTextCharFormat error_format;
    error_format.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);
    error_format.setUnderlineColor(QColor(Qt::red));

    if(spell_checker_.isNull())
        spell_checker_ = SpellCheckerFactory::instance().make(language());

    foreach(Range range, spell_checker_->spellErrors(text)) {
        setFormat(range.location(), range.length(), error_format);
    }
}

bool SpellCheckHighlighter::isEnabled() const {
    return enabled_;
}

Language SpellCheckHighlighter::language() const {
    return lang_;
}

void SpellCheckHighlighter::setEnabled(bool value) {
    enabled_ = value;
}

bool SpellCheckHighlighter::setLanguage(const Language& lang) {
    if(lang_ == lang)
        return true;

    lang_ = lang;

    spell_checker_ = SpellCheckerFactory::instance().make(lang_);

    qDebug() << Q_FUNC_INFO << QString("Spell check language: \"%1\"").arg(lang.name());

    return true;
}

SpellCheckerPtr SpellCheckHighlighter::spellChecker() {
    return spell_checker_;
}
