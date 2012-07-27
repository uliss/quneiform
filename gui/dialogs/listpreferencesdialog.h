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

#ifndef LISTPREFERENCESDIALOG_H
#define LISTPREFERENCESDIALOG_H

#include "abstractpreferencesdialog.h"

class QListWidget;
class QPushButton;

class ListPreferencesDialog : public AbstractPreferencesDialog
{
    Q_OBJECT
public:
    ListPreferencesDialog(QWidget * parent = 0);
    virtual void setCurrentIndex(int idx);
private slots:
    void savePreferences();
    void showCategory(int idx);
protected:
    void setPreferenceActions(const PreferencesList& prefs);
private:
    void initDialogButtons();
    void setupLayout();
    void setupList();
private:
    QPushButton * btn_save_;
    QPushButton * btn_cancel_;
    QListWidget * list_;
};

#endif // LISTPREFERENCESDIALOG_H
