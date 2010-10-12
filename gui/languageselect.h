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

#ifndef LANGUAGE_SELECT_H_
#define LANGUAGE_SELECT_H_

#include <QToolButton>

class QMenu;

class LanguageSelect : public QToolButton {
	Q_OBJECT
public:
	LanguageSelect(QWidget * parent = 0);
	QString currentLanguage() const;
	void select(int langCode);
public:
	static QStringList supportedLanguages();
	static void fillLanguageMenu(QMenu* menu);
private:
	void initLanguages();
private:
	QMenu * menu_;
	int current_language_;
};


#endif // LANGUAGE_SELECT_H_
