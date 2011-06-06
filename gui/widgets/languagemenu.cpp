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

#include "languagemenu.h"
#include "language.h"

LanguageMenu::LanguageMenu(QWidget *parent) :
    QMenu(tr("Language"), parent)
{
    fillActions();
    select(Language::english());
}

void LanguageMenu::fillActions() {
    foreach(Language l, Language::supportedLanguages(Language::BY_TR_NAME)) {
        QAction * lang_action = addAction(l.trName());
        lang_action->setData(QVariant::fromValue(l));
        lang_action->setCheckable(true);
        connect(lang_action, SIGNAL(triggered()), SLOT(selectAction()));
    }
}

void LanguageMenu::selectAction() {
    QAction * act = qobject_cast<QAction*>(sender());
    if(!act)
        return;

    Language lang = act->data().value<Language>();
    select(lang);

    emit languageSelected(lang);
}

void LanguageMenu::select(const Language& lang) {
    blockSignals(true);
    foreach(QAction * act, actions()) {
        Language act_lang = act->data().value<Language>();
        act->setChecked(act_lang == lang);
    }
    blockSignals(false);
}
