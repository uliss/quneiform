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

#include <QPixmap>
#include <QPixmapCache>
#include <QDebug>
#include <QImageReader>

#include "imagecache.h"
#include "imageurl.h"

namespace {
    static const int PIXMAP_CACHE_SIZE_KB = 40 * 1024;
    bool setLimit() {
        qDebug() << "[QPixmapCache::setCacheLimit]" << PIXMAP_CACHE_SIZE_KB << "kb";
        QPixmapCache::setCacheLimit(PIXMAP_CACHE_SIZE_KB);
        return true;
    }

    bool set = setLimit();
}

#if QT_VERSION <= 0x040600
#define ARG_FIX(arg) *arg
#else
#define ARG_FIX(arg) arg
#endif

bool ImageCache::find(const ImageURL& path, QPixmap * pixmap) {
    return QPixmapCache::find(path.id(), ARG_FIX(pixmap));
}

bool ImageCache::find(const QString& path, QPixmap * pixmap)
{
    return find(ImageURL(path), pixmap);
}

bool ImageCache::insert(const ImageURL& path, const QPixmap& pixmap) {
    return QPixmapCache::insert(path.id(), pixmap);
}

bool ImageCache::insert(const QString& path, const QPixmap& pixmap)
{
    return insert(ImageURL(path), pixmap);
}

bool ImageCache::load(const ImageURL& path, QPixmap * pixmap)
{
    if(path.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "empty path given";
        return false;
    }

    if(!pixmap) {
        qWarning() << Q_FUNC_INFO << "NULL image pointer given";
        return false;
    }

    if (!QPixmapCache::find(path.id(), ARG_FIX(pixmap))) {
        if(path.isSimple()) {
            qDebug() << "[ImageCache::load] from file: " << path.id();
            pixmap->load(path.path());
        }
        else {
            QByteArray format = QImageReader::imageFormat(path.path()).toUpper();
            if(format == "TIFF")
                format = "MTIFF";

            QImageReader r(path.path(), format);

            if(!r.jumpToImage(path.imageNumber())) {
                qWarning() << Q_FUNC_INFO << "can't jump to page" << path.imageNumber();
                return false;
            }

            *pixmap = QPixmap::fromImageReader(&r);
        }

        QPixmapCache::insert(path.id(), *pixmap);
    }

    if(pixmap->isNull()) {
        if(path.isSimple()) {
            pixmap->load(path.path());
            if(!pixmap->isNull())
                return true;

            qDebug() << "[ImageCache::load] invalid pixmap: " << path.id();
            return false;
        }
        else {
            QByteArray format = QImageReader::imageFormat(path.path()).toUpper();
            if(format == "TIFF")
                format = "MTIFF";

            QImageReader r(path.path(), format);
            if(!r.jumpToImage(path.imageNumber()))
                return false;

            *pixmap = QPixmap::fromImageReader(&r);
            if(!pixmap->isNull())
                return true;

            qDebug() << Q_FUNC_INFO << "multipage pixmap load failed" << path.id();
            return false;
        }
    }
    else {
        return true;
    }
}

bool ImageCache::load(const QString& path, QPixmap * pixmap)
{
    return load(ImageURL(path), pixmap);
}
