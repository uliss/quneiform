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
#include <QFrame>

#include "tabpreferencesdialog.h"

TabPreferencesDialog::TabPreferencesDialog(QWidget * parent) :
    AbstractPreferencesDialog(parent),
    btn_save_(NULL),
    btn_cancel_(NULL),
    tab_widget_(NULL)
{
    initDialogButtons();
    tab_widget_ = new QTabWidget(this);
    categories()->hide();


    setDialogLayout();
}

void TabPreferencesDialog::loadPreferences()
{
    qDebug() << Q_FUNC_INFO;
}

void TabPreferencesDialog::savePreferences()
{
    qDebug() << Q_FUNC_INFO;
}

void TabPreferencesDialog::showCategory(int idx)
{
    Q_CHECK_PTR(categories());

    if(categories()->count() <= idx) {
        qWarning() << Q_FUNC_INFO << "invalid category index:" << idx;
        return;
    }

    categories()->setCurrentIndex(idx);
}

void TabPreferencesDialog::addCategoryAction(const QIcon& icon, const QString& text, QWidget * widget)
{
    Q_CHECK_PTR(tab_widget_);
    qDebug() << Q_FUNC_INFO;

    tab_widget_->addTab(widget, text);

    connect(tab_widget_, SIGNAL(currentChanged(int)), this, SLOT(showCategory(int)));
}

void TabPreferencesDialog::initDialogButtons()
{
    setDialogButtons(new QDialogButtonBox);
    btn_save_ = dialogButtons()->addButton(QDialogButtonBox::Save);
    btn_cancel_ = dialogButtons()->addButton(QDialogButtonBox::Cancel);
    connect(btn_cancel_, SIGNAL(clicked()), this, SLOT(close()));
}

void TabPreferencesDialog::setDialogLayout()
{
    setLayout(new QVBoxLayout);
    layout()->addWidget(tab_widget_);
    layout()->addWidget(categories());
    layout()->addWidget(dialogButtons());
}
