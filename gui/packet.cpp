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
#include <QCoreApplication>

#include "export/cfexporter.h"
#include "packet.h"
#include "page.h"
#include "thumbnailgenerator.h"
#include "export/cedpagemerge.h"

static const QString DEFAULT_NAME = "untitled.qpf";

Packet::Packet(QObject * parent) :
        QObject(parent),
        filename_(DEFAULT_NAME),
        changed_(false),
        is_new_(true),
        page_remove_lock_(false)
{
}

Packet::~Packet()
{
    if(thumb_generator_)
        thumb_generator_->cancel();
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
    emit pageAdded(page);
    pageChange();
}

void Packet::clear()
{
    if(pages_.isEmpty())
        return;

    if(thumb_generator_)
        thumb_generator_->cancel();

    changed_ = true;

    foreach(Page * p, pages_) {
        emit pageRemoved(p);
        delete p;
    }

    pages_.clear();
    emit changed();
}

bool Packet::exportTo(const QString& fullPath, const ExportSettings& settings)
{
    cf::CEDPageMerge m;

    foreach(Page * p, pages_) {
        m.add(p->cedPage());
    }

    try {
        CfExporter exp(settings);
        exp.exportCEDPage(m.get(), FormatSettings(), fullPath);
    }
    catch(ExporterException& e) {
        qWarning() << Q_FUNC_INFO << e.what();
        return false;
    }

    return true;
}

QString Packet::fileName() const {
    return filename_;
}

Page * Packet::firstPage() {
    return pages_.isEmpty() ? NULL : pages_.first();
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

bool Packet::isEmpty() const {
    return pages_.isEmpty();
}

bool Packet::isNew() const {
    return is_new_;
}

Page * Packet::lastPage()
{
    if(isEmpty())
        return NULL;
    return pages_.last();
}

bool Packet::open(const QString& filename) {
    qDebug() << "[Packet::open]" << filename;

    QFile file(filename);

    try {
        if(file.open(QIODevice::ReadOnly)) {
            QDataStream stream(&file);
            stream.setVersion(QDataStream::Qt_4_5);
            clear();
            stream >> *this;
            if(stream.status() != QDataStream::Ok) {
                qDebug() << "[Packet::open] read error" << filename;
                clear();
                return false;
            }
        }
        else
            return false;

    }
    catch(std::exception& e) {
        qDebug() << Q_FUNC_INFO << " exception: " << e.what();
        return false;
    }

    file.close();
    filename_ = filename;
    changed_ = false;
    is_new_ = false;

    emit opened();

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

void Packet::setupThumbGenerator()
{
    thumb_generator_.reset(new ThumbnailGenerator(this));
    connect(thumb_generator_.data(), SIGNAL(started()), SLOT(thumbGeneratorStart()));
    connect(thumb_generator_.data(), SIGNAL(finished()), SLOT(thumbGeneratorFinish()));
}

void Packet::thumbGeneratorFinish()
{
    qDebug() << Q_FUNC_INFO;
    page_remove_lock_ = false;
    removeDelayed();
}

void Packet::thumbGeneratorStart()
{
    qDebug() << Q_FUNC_INFO;
    page_remove_lock_ = true;
}

void Packet::removeDelayed()
{
    // if no pages were removed during thumb generation
    if(delayed_remove_.isEmpty())
        return;

    foreach(Page * p, delayed_remove_) {
        p->deleteLater();
    }

    delayed_remove_.clear();
    pageChange();
}

int Packet::pageCount() const {
    return pages_.count();
}

int Packet::recognizedPageCount() const
{
    int res = 0;
    foreach(Page * p, pages_) {
        if(p->isRecognized())
            res++;
    }
    return res;
}

void Packet::remove(Page * page)
{
    if (pages_.indexOf(page) < 0) {
        qWarning() << Q_FUNC_INFO << "page not found";
        return;
    }

    emit pageRemoved(page);
    pages_.removeAll(page);

    // thumb generation is running
    if(page_remove_lock_) {
        qWarning() << Q_FUNC_INFO << "delayed page removal:" << page << "cause packet is locked";
        if(!delayed_remove_.contains(page))
            delayed_remove_.append(page);

        if(pages_.isEmpty() && thumb_generator_)
            thumb_generator_->cancel();

        return;
    }

    page->deleteLater();
    pageChange();
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
    else {
        qDebug() << "[Error]" << Q_FUNC_INFO << "can't open file" << filename;
        return false;
    }

    packet.close();
    filename_ = filename;
    changed_ = false;
    is_new_ = false;

    emit saved();
    return true;
}

void Packet::updateThumbs()
{
    if(!thumb_generator_)
        setupThumbGenerator();

    PageList target_pages;
    foreach(Page * p, pages_) {
        if(p->hasThumb())
            continue;

        target_pages.append(p);
    }

    if(target_pages.isEmpty())
        return;

    thumb_generator_->setPages(target_pages);
    thumb_generator_->run();
}

QDataStream& operator<<(QDataStream& os, const Packet& packet) {
    os << packet.pageCount();

    foreach(Page * p, packet.pages_) {
        os << *p;
    }

    return os;
}

QDataStream& operator>>(QDataStream& is, Packet& packet) {
    if(is.status() != QDataStream::Ok)
        return is;

    int page_count = 0;
    is >> page_count;

    if(page_count <= 0)
        return is;

    for(int i = 0; i < page_count; i++) {
        Page * p = new Page("");
        is >> *p;

        if(is.status() != QDataStream::Ok) {
            packet.loaded(100);
            delete p;
            break;
        }

        packet.append(p, true);
        packet.loaded((i * 100) / page_count);

        QCoreApplication::processEvents();
    }

    return is;
}

void Packet::move(int pos, Page * src)
{
    if(pos > pages_.size())
        return;

    if(!pages_.contains(src))
        return;

    pages_.removeAll(src);
    pages_.insert(pos, src);

    pageChange();
    emit reordered();
}

void Packet::moveBefore(Page * page, Page * target) {
    if(page == NULL || target == NULL)
        return;

    if(page == target)
        return;

    if(!pages_.contains(target))
        return;

    move(pages_.indexOf(target), page);
}

