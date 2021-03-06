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

#include <QSettings>
#include <QFileInfo>
#include <QDebug>

#include "recentmenu.h"

RecentMenu::RecentMenu(QWidget * parent, const QString& title, const QString& name, int maxItems) :
    QMenu(title, parent), name_(name), max_items_(maxItems)
{
    Q_ASSERT(maxItems >= 0);
    readSettings();
}

RecentMenu::~RecentMenu() {
    writeSettings();
}

void RecentMenu::add(const QString& file) {
    if(items_.contains(file))
        items_.removeAll(file);

    items_ << file;

    while(items_.size() > max_items_)
        items_.removeFirst();

    fillActions();
}

void RecentMenu::addMenuAction(const QString& path) {
    QFileInfo inf(path);

    // skipping and removing non-exist files
    if(!inf.exists()) {
        qDebug() << QString("[Warning] file \"%1\" not exist. Removing it from recent menu list.").arg(path);
        items_.removeAll(path);
        return;
    }

    QAction * act = addAction(path);
    act->setData(path);
    connect(act, SIGNAL(triggered()), SLOT(selectItem()));
}

void RecentMenu::clear() {
    QMenu::clear();
    items_.clear();
}

void RecentMenu::fillActions() {
    QMenu::clear();

    QStringListIterator i(items_);
    for(i.toBack(); i.hasPrevious(); i.previous())
        addMenuAction(i.peekPrevious());
}

void RecentMenu::readSettings() {
    QSettings settings;
    items_ = settings.value(name_).toStringList();
    fillActions();
}

void RecentMenu::selectItem() {
    QAction * act = qobject_cast<QAction*>(sender());
    if(!act)
        return;

    emit selected(act->data().toString());
}

void RecentMenu::writeSettings() {
    QSettings settings;
    settings.setValue(name_, items_);
}

