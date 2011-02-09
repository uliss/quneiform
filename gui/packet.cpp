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

#include <QDebug>
#include <QFile>
#include "packet.h"
#include "page.h"

static const QString DEFAULT_NAME = "untitled.qpf";

Packet::Packet(QObject * parent) :
        QObject(parent),
        filename_(DEFAULT_NAME),
        language_(-1),
        changed_(false),
        is_new_(true) {
}

Packet::~Packet() {

}

void Packet::append(Page * page, bool allowDuplication) {
    if(!page) {
        qDebug() << "[Packet::append] NULL page pointer given";
        return;
    }

    if(!allowDuplication) {
        if(hasPage(page->imagePath())) {
            emit imageDuplicated(page->imagePath());
            return;
        }
    }

    pages_.append(page);
    page->setParent(this);
    connect(page, SIGNAL(changed()), SLOT(pageChange()));
    changed_ = true;
    emit pageAdded(page);
    emit changed();
    qDebug() << "[Packet::append()]";
}

void Packet::clear() {
    bool has_pages = false;

    if(!pages_.isEmpty()) {
        changed_ = true;
        has_pages = true;
    }

    for(int i = 0; i < pages_.count(); i++) {
        Page * p = pages_.at(i);
        emit pageRemoved(p);
        delete p;
    }

    pages_.clear();

    if(has_pages)
        emit changed();
}

int Packet::countSelected() const {
    int res = 0;
    foreach(Page * page, pages_){
        if (page->isSelected())
            res++;
    }
    return res;
}

QString Packet::fileName() const {
    return filename_;
}

bool Packet::hasPage(const QString& path) const {
    for (PageList::const_iterator it = pages_.begin(); it != pages_.end(); ++it) {
        if ((*it)->imagePath() == path)
            return true;
    }
    return false;
}

bool Packet::isChanged() const {
    return changed_;
}

bool Packet::isNew() const {
    return is_new_;
}

int Packet::language() const {
    return language_;
}

bool Packet::open(const QString& filename) {
    qDebug() << "[Packet::open]" << filename;

    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_4_5);
        clear();
        stream >> *this;
        if(stream.status() != QDataStream::Ok) {
            qDebug() << "[Packet::open] read error" << filename;
            return false;
        }
    }
    else
        return false;

    file.close();
    filename_ = filename;
    changed_ = false;
    is_new_ = false;
    return true;
}

Page * Packet::pageAt(int index) {
    if (0 <= index && index < pages_.size())
        return pages_.at(index);
    else
        return 0;
}

void Packet::pageChange() {
    changed_ = true;
    emit changed();
}

int Packet::pageCount() const {
    return pages_.count();
}

void Packet::remove(Page * page) {
    if (pages_.indexOf(page) < 0)
        return;

    emit pageRemoved(page);
    pages_.removeAll(page);
    delete page;
    pageChange();
    qDebug() << "[Packet::remove()]";
}

void Packet::removeSelected() {
    foreach(Page * page, pages_) {
        if (page->isSelected())
            remove(page);
    }
}

bool Packet::save(const QString& filename) {
    qDebug() << "[Packet::save]" << filename;

    QFile packet(filename);

    if(packet.open(QIODevice::WriteOnly)) {
        QDataStream stream(&packet);
        stream.setVersion(QDataStream::Qt_4_5);
        stream << *this;
        if(stream.status() != QDataStream::Ok) {
            qDebug() << "[Packet::save] write error to file:" << filename;
            return false;
        }
    }
    else
        return false;

    packet.close();
    filename_ = filename;
    changed_ = false;
    is_new_ = false;
    emit saved();
    return true;
}

void Packet::setLanguage(int lang) {
    // TODO check
    language_ = lang;
    emit changed();
}

QDataStream& operator<<(QDataStream& os, const Packet& packet) {
    os << packet.language_;
    os << packet.pageCount();

    foreach(Page * p, packet.pages_) {
        os << *p;
    }

    return os;
}

QDataStream& operator>>(QDataStream& is, Packet& packet) {
    is >> packet.language_;

    int page_count;
    is >> page_count;

    if(page_count > 0) {
        for(int i = 0; i < page_count; i++) {
            Page * p = new Page("");
            is >> *p;
            packet.append(p, true);
        }
    }

    return is;
}
