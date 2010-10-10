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
#include <qdebug.h>
#include "cuneiform.h"
#include "languageselect.h"

LanguageSelect::LanguageSelect(QWidget * parent) : QToolButton(parent) {
	menu_ = new QMenu(this);
	setMenu(menu_);
	setPopupMode(QToolButton::MenuButtonPopup);
	initLanguages();
}

QString LanguageSelect::currentLanguage() const {
	return "en";
}

void LanguageSelect::initLanguages() {
	Q_CHECK_PTR(menu_);
	using namespace cf;
	LanguageList langs = AlphabetFactory::instance().supportedLanguages();
	Language::sortByName(langs);
	for (LanguageList::iterator it = langs.begin(), end = langs.end(); it
			!= end; ++it) {
		QAction * lang_action = menu_->addAction(QString(Language::isoName(*it).c_str()), this, SLOT(selected()));
		lang_action->setData(*it);
	}
}

void LanguageSelect::selected() {
	//current_language_ = sender()->data().toInt();
}

QStringList LanguageSelect::supportedLanguages() {
	QStringList res;
	using namespace cf;
	LanguageList langs = AlphabetFactory::instance().supportedLanguages();
	Language::sortByName(langs);
	for (LanguageList::iterator it = langs.begin(), end = langs.end(); it
			!= end; ++it) {
		res << Language::isoName(*it).c_str();
	}
	return res;
}
