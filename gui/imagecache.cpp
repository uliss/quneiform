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
#include "imagecache.h"

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

bool ImageCache::find(const QString &path, QPixmap * pixmap) {
    return QPixmapCache::find(path, ARG_FIX(pixmap));
}

bool ImageCache::insert(const QString& path, const QPixmap& pixmap) {
    return QPixmapCache::insert(path, pixmap);
}

bool ImageCache::load (const QString &path, QPixmap * pixmap)
{
    if(path.isEmpty())
        return false;

    if(!pixmap) {
        qDebug() << "[ImageCache::find] NULL pointer given";
        return false;
    }

    if (!QPixmapCache::find(path, ARG_FIX(pixmap))) {
        qDebug() << "[ImageCache::load] from file: " << path;
        pixmap->load(path);
        QPixmapCache::insert(path, *pixmap);
    }

    if(pixmap->isNull()) {
        qDebug() << "[ImageCache::load] invalid pixmap: " << path;
        return false;
    }
    else {
        return true;
    }
}
