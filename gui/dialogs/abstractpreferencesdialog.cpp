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

#include "abstractpreferencesdialog.h"

AbstractPreferencesDialog::AbstractPreferencesDialog(QWidget *parent) :
    QDialog(parent),
    categories_(NULL),
    buttons_(NULL)
{
    setWindowTitle("Settings");
    categories_ = new QStackedWidget(this);
}

void AbstractPreferencesDialog::addCategoryWidget(const QIcon& icon, const QString& text, QWidget * widget)
{
    if(!widget) {
        qCritical() << Q_FUNC_INFO << "NULL widget given";
        return;
    }

    categories_->addWidget(widget);
    addCategoryAction(icon, text, widget);
}

QStackedWidget *AbstractPreferencesDialog::categories()
{
    return categories_;
}

QDialogButtonBox * AbstractPreferencesDialog::dialogButtons()
{
    return buttons_;
}

void AbstractPreferencesDialog::setDialogButtons(QDialogButtonBox * buttons)
{
    buttons_ = buttons;
    buttons_->setParent(this);
}
