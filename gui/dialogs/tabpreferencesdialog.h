/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#ifndef TABPREFERENCESDIALOG_H
#define TABPREFERENCESDIALOG_H

#include "abstractpreferencesdialog.h"

class QPushButton;
class QTabWidget;
class QFrame;
class QAction;

class TabPreferencesDialog : public AbstractPreferencesDialog
{
    Q_OBJECT
public:
    TabPreferencesDialog(QWidget * parent = 0);
protected:
    void setPreferenceActions(const PreferencesList& pages);
private:
    void initDialogButtons();
    void setupLayout();
private slots:
    void savePreferences();
    void saveCurrentPage();
private:
    QPushButton * btn_save_;
    QPushButton * btn_cancel_;
    QTabWidget * tab_widget_;
    PreferencesList prefs_;
};

#endif // TABPREFERENCESDIALOG_H
