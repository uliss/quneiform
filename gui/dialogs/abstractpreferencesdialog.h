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

#ifndef ABSTRACTPREFERENCESDIALOG_H
#define ABSTRACTPREFERENCESDIALOG_H

#include <QDialog>
#include <QList>

class QStackedWidget;
class QDialogButtonBox;
class PreferencesWidget;

typedef QList<PreferencesWidget*> PreferencesList;

class AbstractPreferencesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AbstractPreferencesDialog(QWidget * parent = 0);
    ~AbstractPreferencesDialog();

    virtual PreferencesList pages() const;
    virtual void setPages(const PreferencesList& pages);

    QDialogButtonBox * dialogButtons();
    void setDialogButtons(QDialogButtonBox * buttons);

    virtual void setCurrentIndex(int idx);
    virtual void setCurrentPage(PreferencesWidget * pref);
protected:
    virtual void setPreferenceActions(const PreferencesList& pages) = 0;
private slots:
    void loadCurrentPage();
    void saveCurrentPage();
private:
    void setupActions();
protected:
    QStackedWidget * stack_widget_;
private:
    QDialogButtonBox * buttons_;
    QAction * act_close_;
};

#endif // ABSTRACTPREFERENCESDIALOG_H
