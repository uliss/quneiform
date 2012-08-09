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
#include <QMutex>
#include <QMutexLocker>

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

    QMutex image_cache_mutex;
    int image_cache_current_size = 0;
    int image_cache_limit = 10 * 1024 * 1024;

    int freeSpace()
    {
        return ImageCache::cacheLimit() - image_cache_current_size;
    }

    bool haveSpace(int memory)
    {
        return freeSpace() >= memory;
    }

    bool loadImage(const ImageURL& path, QImage * image)
    {
        QByteArray format = QImageReader::imageFormat(path.path()).toUpper();
        if(format == "TIFF")
            format = "MTIFF";

        QImageReader r(path.path(), format);
        if(format == "PDF")
            r.setQuality(300);

        if(!path.isSimple()) {
            if(!r.jumpToImage(path.imageNumber())) {
                qWarning() << Q_FUNC_INFO << "can't jump to page" << path.imageNumber();
                return false;
            }
        }

        return r.read(image);
    }
}

#if QT_VERSION <= 0x040600
#define ARG_FIX(arg) *arg
#else
#define ARG_FIX(arg) arg
#endif

ImageCache::ImageMap ImageCache::map_;
ImageCache::ImageQueue ImageCache::queue_;

bool ImageCache::find(const ImageURL& path, QPixmap * pixmap)
{
    return QPixmapCache::find(path.id(), ARG_FIX(pixmap));
}

bool ImageCache::find(const QString& path, QPixmap * pixmap)
{
    return find(ImageURL(path), pixmap);
}

bool ImageCache::find(const ImageURL& path, QImage * image)
{
    return find(path.id(), image);
}

bool ImageCache::find(const QString& path, QImage * image)
{
    QMutexLocker lock(&image_cache_mutex);

    ImageMap::iterator it = map_.find(path);
    bool rc = (it != map_.end());

    if(rc && image != NULL)
        *image = *it;

    return rc;
}

bool ImageCache::insert(const ImageURL& path, const QPixmap& pixmap) {
    return QPixmapCache::insert(path.id(), pixmap);
}

bool ImageCache::insert(const QString& path, const QPixmap& pixmap)
{
    return insert(ImageURL(path), pixmap);
}

bool ImageCache::insert(const ImageURL& path, const QImage& image)
{
    return insert(path.id(), image);
}

bool ImageCache::insert(const QString& path, const QImage& image)
{
    if(image.isNull()) {
        qWarning() << Q_FUNC_INFO << "attempt to insert NULL image with key:" << path;
        return false;
    }

    if(path.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "empty key given";
        return false;
    }

    QMutexLocker lock(&image_cache_mutex);

    ImageMap::iterator it = map_.find(path);

    if(it == map_.end()) { // first insert
        // have free space
        if(haveSpace(image.byteCount())) {
            insertNoCheck(path, image);
            return true;
        }
        else { // no space - have to delete oldest
            if(map_.isEmpty()) { // nothing to delete, image is too big
                qWarning() << Q_FUNC_INFO << "can't insert image, to space for"
                           << image.byteCount() << "bytes";
                return false;
            }

            if(!freeMemory(image.byteCount())) { // still no space
                qWarning() << Q_FUNC_INFO << "no space for replacing image with key:" << path;
                return false;
            }

            insertNoCheck(path, image);
            return true;
        }
    }
    else { // replace existing
        int size_diff = image.byteCount() - it->byteCount();

        if(size_diff <= 0) { // same size or less
            // move to queue tail
            queue_.removeOne(path);
            queue_.enqueue(path);
            // decrease size
            image_cache_current_size += size_diff;
            Q_ASSERT(image_cache_current_size >= 0);
            *it = image;
            return true;
        }

        // inserted image less then previous
        if(haveSpace(size_diff)) { // have space to increase
            // move to queue tail
            queue_.removeOne(path);
            queue_.enqueue(path);
            // increase size
            image_cache_current_size += size_diff;
            Q_ASSERT(image_cache_current_size <= image_cache_limit);
            *it = image;
            return true;
        }
        else { // no space
            if(!freeMemory(size_diff))
                return false;

            // yes, we have space
            // move to queue tail - newest
            queue_.removeOne(path);
            queue_.enqueue(path);
            image_cache_current_size += size_diff;
            *it = image;
            return true;
        }
    }

    return false;
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

    // pixmap not found
    if(!QPixmapCache::find(path.id(), ARG_FIX(pixmap))) {
        QImage img;
        if(!loadImage(path, &img)) {
            qDebug() << Q_FUNC_INFO << "can't load image:" << path.id();
            return false;
        }

        *pixmap = QPixmap::fromImage(img);

        if(!QPixmapCache::insert(path.id(), *pixmap))
            qDebug() << Q_FUNC_INFO << "can't insert into cache:" << path.id();

        return true;
    }

    // found valid pixmap
    if(!pixmap->isNull())
        return true;

    // found NULL pixmap
    QImage img;
    if(!loadImage(path, &img)) {
        qDebug() << Q_FUNC_INFO << "can't load image:" << path.id();
        return false;
    }

    *pixmap = QPixmap::fromImage(img);

    if(!QPixmapCache::insert(path.id(), *pixmap))
        qDebug() << Q_FUNC_INFO << "can't insert into cache:" << path.id();

    return true;
}

bool ImageCache::load(const QString& path, QPixmap * pixmap)
{
    return load(ImageURL(path), pixmap);
}

bool ImageCache::load(const ImageURL& path, QImage * image)
{
    if(path.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "empty path given";
        return false;
    }

    if(!image) {
        qWarning() << Q_FUNC_INFO << "NULL image pointer given";
        return false;
    }

    if(find(path, image))
        return true;

    {
        QMutexLocker lock(&image_cache_mutex);
        bool rc = loadImage(path, image);
        if(!rc) {
            qDebug() << Q_FUNC_INFO << "can't load image:" << path.id();
            return false;
        }
    }

    if(!insert(path, *image))
        qDebug() << Q_FUNC_INFO << "can't insert loaded image into cache:" << path.id();

    return true;
}

bool ImageCache::load(const QString& path, QImage * image)
{
    return load(ImageURL(path), image);
}

bool ImageCache::freeMemory(int requiredSize)
{
    if(requiredSize <= 0) {
        qWarning() << Q_FUNC_INFO << "invalid memory size:" << requiredSize;
        return false;
    }

    int object_to_remove = 0;
    int objects_size = 0;

    foreach(QString key, queue_) {
        objects_size += map_.value(key).byteCount();
        object_to_remove++;

        if(objects_size >= requiredSize)
            break;
    }

    if(objects_size < requiredSize) {
        qDebug() << Q_FUNC_INFO << "can't free required size:" << requiredSize;
        // exit but nothing removed
        return false;
    }

    // remove images
    for(int i = 0; i < object_to_remove; i++) {
        QString key = queue_.at(i);
        map_.remove(key);
    }

    // removes from queue
    for(int i = 0; i < object_to_remove; i++)
        queue_.removeFirst();

    // decrease current size
    image_cache_current_size -= objects_size;
    Q_ASSERT(image_cache_current_size >= 0);

    return true;
}

void ImageCache::insertNoCheck(const QString &path, const QImage &image)
{
    map_.insert(path, image);
    queue_.enqueue(path);
    image_cache_current_size += image.byteCount();
}

size_t ImageCache::cacheLimit()
{
    return image_cache_limit;
}

void ImageCache::setCacheLimit(size_t limit)
{
    QMutexLocker lock(&image_cache_mutex);
    image_cache_limit = limit;
}

void ImageCache::clear()
{
    QMutexLocker lock(&image_cache_mutex);
    queue_.clear();
    map_.clear();
    image_cache_current_size = 0;
}
