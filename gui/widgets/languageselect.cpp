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
#include "languageselect.h"

LanguageSelect::LanguageSelect(QWidget * parent)
    : QComboBox(parent)
{
    initLanguages();
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(selectLanguage(int)));
    select(Language::english());
}

Language LanguageSelect::currentLanguage() const {
    QVariant var = itemData(currentIndex());
    return var.isValid() ? Language(var.toInt()) : Language();
}

void LanguageSelect::initLanguages() {
    foreach(Language l, Language::supportedLanguages(Language::BY_TR_NAME)) {
        addItem(l.trName(), l.code());
    }
}

void LanguageSelect::selectLanguage(int item_index) {
    if(item_index < 0)
        return;

    QVariant data = itemData(item_index);
    if(!data.isValid())
        return;

    emit languageSelected(Language(data.toInt()));
}

void LanguageSelect::select(const Language&  lang) {
    blockSignals(true);
    int item_idx = findData(lang.code());
    if(item_idx != -1)
        setCurrentIndex(item_idx);
    else
        qDebug() << "[Warning]" << Q_FUNC_INFO << "Language" << lang.name() << "not found";
    blockSignals(false);
}
