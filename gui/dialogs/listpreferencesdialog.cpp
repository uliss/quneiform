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

#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QStackedWidget>
#include <QDebug>

#include "listpreferencesdialog.h"
#include "preferenceswidget.h"

ListPreferencesDialog::ListPreferencesDialog(QWidget * parent) :
    AbstractPreferencesDialog(parent),
    btn_save_(NULL),
    btn_cancel_(NULL),
    list_(NULL)
{
    initDialogButtons();
    setupList();
    setupLayout();
}

void ListPreferencesDialog::setCurrentIndex(int idx)
{
    AbstractPreferencesDialog::setCurrentIndex(idx);
    list_->blockSignals(true);
    list_->setCurrentRow(idx);
    list_->blockSignals(false);
}

void ListPreferencesDialog::savePreferences()
{
    foreach(PreferencesWidget * w, pages()) {
        w->saveAll();
    }

    setResult(QDialog::Accepted);
    close();
}

void ListPreferencesDialog::showCategory(int idx)
{
    setCurrentIndex(idx);
}

void ListPreferencesDialog::setPreferenceActions(const PreferencesList& prefs)
{
    foreach(PreferencesWidget * w, prefs) {
        if(!w)
            continue;

        list_->addItem(new QListWidgetItem(w->icon(), w->title()));
    }
}

void ListPreferencesDialog::initDialogButtons()
{
    setDialogButtons(new QDialogButtonBox);
    btn_save_ = dialogButtons()->addButton(QDialogButtonBox::Save);
    btn_cancel_ = dialogButtons()->addButton(QDialogButtonBox::Cancel);
    connect(btn_cancel_, SIGNAL(clicked()), this, SLOT(close()));
    connect(btn_save_, SIGNAL(clicked()), this, SLOT(savePreferences()));
}

void ListPreferencesDialog::setupLayout()
{
    setLayout(new QVBoxLayout);

    QLayout * l = new QHBoxLayout;
    l->setSpacing(10);
    l->addWidget(list_);
    l->addWidget(stack_widget_);

    layout()->addItem(l);
    layout()->addWidget(dialogButtons());
}

void ListPreferencesDialog::setupList()
{
    const int LIST_WIDTH = 150;
    list_ = new QListWidget(this);
    list_->setFixedWidth(LIST_WIDTH);
    connect(list_, SIGNAL(currentRowChanged(int)), this, SLOT(showCategory(int)));
}
