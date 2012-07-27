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

#include <QDebug>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QStackedWidget>
#include <QPushButton>
#include <QTabWidget>
#include <QAction>
#include <QSettings>

#include "tabpreferencesdialog.h"
#include "preferenceswidget.h"
#include "settingskeys.h"

static const QString KEY_CURRENT_TAB_PAGE = QString("%1_tab").arg(KEY_SHOW_CURRENT_CHAR);

TabPreferencesDialog::TabPreferencesDialog(QWidget * parent) :
    AbstractPreferencesDialog(parent),
    btn_save_(NULL),
    btn_cancel_(NULL),
    tab_widget_(NULL)
{
    initDialogButtons();
    setupLayout();
}

void TabPreferencesDialog::savePreferences()
{
    foreach(PreferencesWidget * w, prefs_) {
        if(!w)
            continue;

        w->saveAll();
    }

    setResult(QDialog::Accepted);
    close();
}

void TabPreferencesDialog::setPreferenceActions(const PreferencesList& pages)
{
    Q_CHECK_PTR(tab_widget_);

    int idx = QSettings().value(KEY_CURRENT_TAB_PAGE, 0).toInt();
    foreach(PreferencesWidget * w, pages) {
        tab_widget_->addTab(w, w->icon(), w->title());
    }

    prefs_ = pages;
    tab_widget_->setCurrentIndex(idx);
}

void TabPreferencesDialog::initDialogButtons()
{
    setDialogButtons(new QDialogButtonBox);
    btn_save_ = dialogButtons()->addButton(QDialogButtonBox::Save);
    btn_cancel_ = dialogButtons()->addButton(QDialogButtonBox::Cancel);
    connect(btn_cancel_, SIGNAL(clicked()), this, SLOT(close()));
    connect(btn_save_, SIGNAL(clicked()), this, SLOT(savePreferences()));
}

void TabPreferencesDialog::setupLayout()
{
    tab_widget_ = new QTabWidget(this);
    connect(tab_widget_, SIGNAL(currentChanged(int)), SLOT(saveCurrentPage()));
    stack_widget_->hide();

    setLayout(new QVBoxLayout);
    layout()->addWidget(tab_widget_);
    layout()->addWidget(dialogButtons());
}

void TabPreferencesDialog::saveCurrentPage()
{
    Q_CHECK_PTR(tab_widget_);
    QSettings().setValue(KEY_CURRENT_TAB_PAGE, tab_widget_->currentIndex());
}
