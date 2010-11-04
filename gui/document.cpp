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
#include "document.h"
#include "page.h"


Document::Document(QObject * parent) :
        QObject(parent), language_(-1), changed_(false) {
}

Document::~Document() {

}

void Document::append(Page * page, bool allowDuplication) {
    if(!allowDuplication) {
        for (PageList::const_iterator it = pages_.begin(); it != pages_.end(); ++it) {
            if ((*it)->imagePath() == page->imagePath()) {
                emit imageDuplicated(page->imagePath());
                return;
            }
        }
    }

    pages_.append(page);
    page->setParent(this);
    connect(page, SIGNAL(changed()), SLOT(pageChange()));
    changed_ = true;
    emit pageAdded(page);
    emit changed();
    qDebug() << "[Document::append()]";
}

void Document::clear() {
    for(int i = 0; i < pages_.count(); i++) {
        Page * p = pages_.at(i);
        emit pageRemoved(p);
        delete p;
    }

    pages_.clear();
}

int Document::countSelected() const {
    int res = 0;
    foreach(Page * page, pages_){
        if (page->isSelected())
            res++;
    }
    return res;
}

QString Document::fileName() const {
    return filename_;
}

bool Document::isChanged() const {
    return changed_;
}

int Document::language() const {
    return language_;
}

bool Document::open(const QString& filename) {
    qDebug() << "[Document::open]" << filename;

    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        stream.setVersion(QDataStream::Qt_4_5);
        clear();
        stream >> *this;
        if(stream.status() != QDataStream::Ok) {
            qDebug() << "[Document::open] read error" << filename;
            return false;
        }
    }
    else
        return false;

    file.close();
    filename_ = filename;
    changed_ = false;
    return true;
}

Page * Document::page(int index) {
    if (0 <= index && index < pages_.size())
        return pages_.at(index);
    else
        return 0;
}

void Document::pageChange() {
    changed_ = true;
    emit changed();
}

int Document::pageCount() const {
    return pages_.count();
}

void Document::recognize(Page * page) {
    Q_CHECK_PTR(page);
    page->recognize();
    emit pageRecognized(page);
}

void Document::recognizeAll() {
    foreach(Page * page, pages_) {
    	recognize(page);
    }

    emit allPagesRecognized();
}

void Document::recognizeSelected() {
    foreach(Page * page, pages_) {
    	if (!page->isSelected())
            continue;
    	recognize(page);
    }

    emit allPagesRecognized();
}

void Document::remove(Page * page) {
    if (pages_.indexOf(page) < 0)
        return;

    emit pageRemoved(page);
    pages_.removeAll(page);
    delete page;
    pageChange();
    qDebug() << "[Document::remove()]";
}

void Document::removeSelected() {
    foreach(Page * page, pages_) {
        if (page->isSelected())
            remove(page);
    }
}

bool Document::save(const QString &filename) {
    qDebug() << "[Document::save]" << filename;

    QFile packet(filename);

    if(packet.open(QIODevice::WriteOnly)) {
        QDataStream stream(&packet);
        stream.setVersion(QDataStream::Qt_4_5);
        stream << *this;
        if(stream.status() != QDataStream::Ok) {
            qDebug() << "[Document::save] write error to file:" << filename;
            return false;
        }
    }
    else
        return false;

    packet.close();
    filename_ = filename;
    changed_ = false;
    emit saved();
    return true;
}

void Document::setLanguage(int lang) {
    // TODO check
    language_ = lang;
}

QDataStream& operator<<(QDataStream& os, const Document& doc) {
    os << doc.pageCount();
    foreach(Page * p, doc.pages_) {
        os << *p;
    }

    os << doc.language_;
    return os;
}

QDataStream& operator>>(QDataStream& is, Document& doc) {
    int page_count;
    is >> page_count;

    if(page_count < 0) {

    }
    else {
        for(int i = 0; i < page_count; i++) {
            Page * p = new Page("");
            is >> *p;
            doc.append(p);
        }

        is >> doc.language_;
    }

    return is;
}

