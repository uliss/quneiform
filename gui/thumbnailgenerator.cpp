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

#include <QtConcurrentMap>
#include <QCoreApplication>

#include "thumbnailgenerator.h"
#include "imagecache.h"
#include "page.h"
#include "packet.h"

ThumbnailGenerator::ThumbnailGenerator(QObject * parent) :
    QObject(parent)
{
    connect(&watcher_, SIGNAL(finished()), SIGNAL(finished()));
    connect(&watcher_, SIGNAL(started()), SIGNAL(started()));
}

ThumbnailGenerator::~ThumbnailGenerator()
{
    job_.cancel();
    job_.waitForFinished();
}

QList<Page*> ThumbnailGenerator::pages() const
{
    return pages_;
}

void ThumbnailGenerator::setPages(const QList<Page*>& pages)
{
    pages_ = pages;
}

void ThumbnailGenerator::cancel()
{
    if(!isRunning())
        return;

    job_.cancel();
    job_.waitForFinished();
    QCoreApplication::processEvents();
    pages_.clear();
}

bool ThumbnailGenerator::isRunning() const
{
    return job_.isRunning();
}

static void makeThumb(Page * p)
{
    if(p->hasThumb())
        return;

    QImage image;
    ImageCache::load(p->imageURL(), &image);
    p->setThumb(image.scaled(Page::maxThumbnailSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    p->setImageSize(image.size());

    qDebug() << Q_FUNC_INFO << "making thumb for:" << p->imageURL();
}

void ThumbnailGenerator::run()
{
    if(pages_.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "no pages";
        return;
    }

    job_ = QtConcurrent::map(pages_.begin(), pages_.end(), &makeThumb);
    watcher_.setFuture(job_);
}
