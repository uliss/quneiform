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
#include <QDebug>

#include "recentpackets.h"

static const int MAX_RECENT_PACKETS = 5;

RecentPackets::RecentPackets(QWidget * parent) : QMenu(tr("Open recent packets"), parent)
{
    readSettings();
}

RecentPackets::~RecentPackets() {
    writeSettings();
}

void RecentPackets::add(const QString& file) {
    if(packets_.contains(file))
        packets_.removeAll(file);

    packets_ << file;

    while(packets_.size() > MAX_RECENT_PACKETS)
        packets_.removeFirst();

    fillActions();
}

void RecentPackets::addMenuAction(const QString& path) {
    QAction * act = addAction(path);
    act->setData(path);
    connect(act, SIGNAL(triggered()), SLOT(selectPacket()));
}

void RecentPackets::fillActions() {
    clear();

    QStringListIterator i(packets_);
    for(i.toBack(); i.hasPrevious(); i.previous())
        addMenuAction(i.peekPrevious());
}

void RecentPackets::readSettings() {
    QSettings settings;
    packets_ = settings.value("recent-packets").toStringList();
    fillActions();
}

void RecentPackets::selectPacket() {
    QAction * act = qobject_cast<QAction*>(sender());
    if(!act)
        return;

    emit selected(act->data().toString());
}

void RecentPackets::writeSettings() {
    QSettings settings;
    settings.setValue("recent-packets", packets_);
}
