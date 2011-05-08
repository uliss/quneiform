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

#ifndef LANGUAGEMENU_H
#define LANGUAGEMENU_H

#include <QMenu>

class Language;

class LanguageMenu : public QMenu
{
    Q_OBJECT
public:
    explicit LanguageMenu(QWidget *parent = 0);
    void select(const Language& lang);
signals:
    void languageSelected(const Language& lang);
private:
    void fillActions();
private slots:
    void selectAction();
};

#endif // LANGUAGEMENU_H
