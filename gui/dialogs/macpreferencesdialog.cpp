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

#include <QAction>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QCoreApplication>
#include <QResizeEvent>
#include <QIcon>
#include <QDebug>

#include "macpreferencesdialog.h"
#include "widgets/preferenceswidget.h"
#include "macosx/mactoolbar.h"

static const int TOOLBAR_HEIGHT = 64;

static QList<QAction*> toActionList(QWidget * parent, const PreferencesList& prefs)
{
    QList<QAction*> res;
    foreach(PreferencesWidget * w, prefs) {
        if(!w)
            continue;

        QAction * a = new QAction(parent);
        a->setText(w->title());
        a->setIcon(w->icon());
        res.append(a);

        QObject::connect(a, SIGNAL(triggered()), parent, SLOT(showCategory()));
    }
    return res;
}

MacPreferencesDialog::MacPreferencesDialog(QWidget * parent) :
    AbstractPreferencesDialog(parent)
{
    setupLayout();
}

MacPreferencesDialog::~MacPreferencesDialog()
{
    macRemoveToolBar(this);
}

void MacPreferencesDialog::setPreferenceActions(const PreferencesList& pages)
{
    macAddToolBar(this, toActionList(this, pages));
    connectPreferences();
}

void MacPreferencesDialog::setCurrentIndex(int idx)
{
    macToolBarSelect(this, idx);
    AbstractPreferencesDialog::setCurrentIndex(idx);
}

void MacPreferencesDialog::showCategory()
{
    QAction * action = qobject_cast<QAction*>(sender());
    if(!action) {
        qWarning() << Q_FUNC_INFO << "not action sender";
        return;
    }

    foreach(PreferencesWidget * w, pages()) {
        if(!w)
            continue;

        if(w->title() == action->text()) {
            setCurrentPage(w);
            break;
        }
    }
}

void MacPreferencesDialog::connectPreferences()
{
    foreach(PreferencesWidget * w, pages()) {
        if(!w)
            continue;

        connect(w, SIGNAL(changed(QWidget*)), w, SLOT(save(QWidget*)));
    }
}

void MacPreferencesDialog::setupLayout()
{
    setLayout(new QVBoxLayout);
    layout()->addWidget(stack_widget_);
}
