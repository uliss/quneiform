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

#include <QMenu>
#include <QAction>
#include <QDebug>
#include "cuneiform.h"
#include "languageselect.h"
#include "language_i18n.h"

LanguageSelect::LanguageSelect(QWidget * parent) : QComboBox(parent) {
    initLanguages();
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(languageChange(int)));
}

int LanguageSelect::currentLanguage() const {
    QVariant l = itemData(currentIndex());
    return l.isValid() ? l.toInt() : LANGUAGE_ENGLISH;
}

void LanguageSelect::initLanguages() {
    LanguageMap langs = supportedLanguages();
    for(LanguageMap::iterator it = langs.begin(), end = langs.end(); it != end; ++it)
        addItem(it.key(), it.value());
}

void LanguageSelect::languageChange(int item_index) {
    if(item_index < 0)
        return;

    QVariant data = itemData(item_index);
    if(!data.isValid())
        return;

    emit languageSelected(data.toInt());
}

void LanguageSelect::select(int lang) {
    int item_idx = findData(QVariant(lang));
    if(item_idx != -1)
        setCurrentIndex(item_idx);
}
