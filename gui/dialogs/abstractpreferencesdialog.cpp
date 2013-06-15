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
#include <QStackedWidget>
#include <QDialogButtonBox>
#include <QLayout>
#include <QSettings>
#include <QAction>

#include "abstractpreferencesdialog.h"
#include "preferenceswidget.h"
#include "settingskeys.h"
#include "iconutils.h"

static const QSizePolicy ignored(QSizePolicy::Ignored, QSizePolicy::Ignored);
static const QSizePolicy preferred(QSizePolicy::Preferred, QSizePolicy::Preferred);

AbstractPreferencesDialog::AbstractPreferencesDialog(QWidget * parent) :
    QDialog(parent),
    stack_widget_(NULL),
    buttons_(NULL),
    act_close_(NULL)
{
    setWindowTitle(tr("Settings"));
    setupActions();
    stack_widget_ = new QStackedWidget(this);
}

AbstractPreferencesDialog::~AbstractPreferencesDialog()
{
    if(stack_widget_)
        QSettings().setValue(KEY_SETTINGS_LAST_PAGE, stack_widget_->currentIndex());
}

void AbstractPreferencesDialog::setPages(const PreferencesList& preferences)
{
    foreach(PreferencesWidget * p, preferences) {
        if(!p) {
            qCritical() << Q_FUNC_INFO << "NULL widget given";
            continue;
        }
        stack_widget_->addWidget(p);
    }

    setPreferenceActions(preferences);
    adjustSize();
    loadCurrentPage();
}

PreferencesList AbstractPreferencesDialog::pages() const
{
    PreferencesList res;
    for(int i = 0; i < stack_widget_->count(); i++) {
        PreferencesWidget * pref = qobject_cast<PreferencesWidget*>(stack_widget_->widget(i));
        if(pref)
            res.append(pref);
    }

    return res;
}

QDialogButtonBox * AbstractPreferencesDialog::dialogButtons()
{
    return buttons_;
}

void AbstractPreferencesDialog::setCurrentIndex(int idx)
{
    Q_CHECK_PTR(stack_widget_);

    if(idx < 0 || stack_widget_->count() <= idx)
        return;

    if(!stack_widget_->count())
        return;

    for(int i = 0; i < stack_widget_->count(); i++) {
        if(i == idx)
            stack_widget_->widget(i)->setSizePolicy(preferred);
        else
            stack_widget_->widget(i)->setSizePolicy(ignored);
    }

    stack_widget_->setCurrentIndex(idx);
    adjustSize();
}

void AbstractPreferencesDialog::setCurrentPage(PreferencesWidget * pref)
{
    Q_CHECK_PTR(stack_widget_);

    if(!pref) {
        qWarning() << Q_FUNC_INFO << "NULL widget given";
        return;
    }

    for(int i = 0; i < stack_widget_->count(); i++) {
        if(stack_widget_->widget(i) == (QWidget*) pref)
            stack_widget_->widget(i)->setSizePolicy(preferred);
        else
            stack_widget_->widget(i)->setSizePolicy(ignored);
    }

    stack_widget_->setCurrentWidget(pref);
    adjustSize();
}

QSize AbstractPreferencesDialog::sizeHint() const
{
    return QSize(550, 100);
}

void AbstractPreferencesDialog::loadCurrentPage()
{
    setCurrentIndex(QSettings().value(KEY_SETTINGS_LAST_PAGE, 0).toInt());
}

void AbstractPreferencesDialog::saveCurrentPage()
{
    Q_CHECK_PTR(stack_widget_);
    QSettings().setValue(KEY_SETTINGS_LAST_PAGE, stack_widget_->currentIndex());
}

void AbstractPreferencesDialog::setupActions()
{
    act_close_ = new QAction("Close", this);
    act_close_->setShortcut(QKeySequence::Close);
    addAction(act_close_);
    connect(act_close_, SIGNAL(triggered()), this, SLOT(close()));
}

void AbstractPreferencesDialog::setDialogButtons(QDialogButtonBox * buttons)
{
    buttons_ = buttons;
    buttons_->setParent(this);
}
