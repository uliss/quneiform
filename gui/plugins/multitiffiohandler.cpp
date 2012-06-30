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

#include <tiffio.h>
#include <cstdio>

#include <QVariant>
#include <QBuffer>
#include <QDebug>
#include <QImage>

#include "multitiffiohandler.h"

static tsize_t cf_read(thandle_t h, tdata_t data, tsize_t size)
{
    QIODevice * device = static_cast<QIODevice*>(h);
    return (tsize_t) device->read(static_cast<char*>(data), size);
}

static tsize_t cf_write(thandle_t h, tdata_t data, tsize_t size)
{
    QIODevice * device = static_cast<QIODevice*>(h);
    return (tsize_t) device->write(static_cast<char*>(data), size);
}

static toff_t cf_seek(thandle_t h, toff_t offset, int whence)
{
    QIODevice * device = static_cast<QIODevice*>(h);
    qint64 pos = device->pos();
    switch(whence) {
    case SEEK_CUR:
        pos += offset;
        break;
    case SEEK_SET:
        pos = offset;
        break;
    case SEEK_END:
        pos = device->size() + offset;
        break;
    default:
        break;
    }

    if (device->seek(pos))
        return (toff_t) pos;

    return (toff_t) device->pos();
}

static int cf_close(thandle_t h)
{
    return 0;
}

static toff_t cf_size(thandle_t handle)
{
    QIODevice * device = static_cast<QIODevice*>(handle);
    return static_cast<toff_t>(device->size());
}

static TIFF * tiffOpen(QIODevice * device)
{
    if(!device)
        return NULL;

    device->reset();
    return TIFFClientOpen("multitiff",
                          "r",
                          device,
                          cf_read,
                          cf_write,
                          cf_seek,
                          cf_close,
                          cf_size,
                          NULL,
                          NULL);
}

static size_t tiffImageCount(QIODevice * device)
{
    if(!device)
        return 0;

    TIFF * tiff = tiffOpen(device);

    if (!tiff) {
        qWarning() << Q_FUNC_INFO << "can't open tiff image";
        return 0;
    }

    uint16 dircount = TIFFNumberOfDirectories(tiff);
    uint16 pages = 0;

    for(uint16 i = 0; i < dircount; i++) {
        if(!TIFFSetDirectory(tiff, i)){
            qWarning() << Q_FUNC_INFO << "can't set tiff directory:" << i;
            return 0;
        }

        uint16 page_number = 0;
        uint16 page_total = 0;

        if(TIFFGetField(tiff, TIFFTAG_PAGENUMBER, &page_number, &page_total)) {
            pages++;
            continue;
        }

        uint32 subfiletype = 0;

        // new subfiletype
        if(TIFFGetField(tiff, TIFFTAG_SUBFILETYPE, &subfiletype)) {
            if(((subfiletype & FILETYPE_PAGE) != 0) || (subfiletype == 0))
                pages++;

            continue;
        }

        // old subfiletype
        if(TIFFGetField(tiff, TIFFTAG_OSUBFILETYPE, &subfiletype)) {
            if((subfiletype == OFILETYPE_IMAGE)
                || (subfiletype == OFILETYPE_PAGE)
                || (subfiletype == 0)) {
                pages++;
            }

            continue;
        }

        pages++;
    }

    TIFFClose(tiff);
    return pages;
}


MultiTIFFIOHandler::MultiTIFFIOHandler() :
    image_num_(0)
{
}

bool MultiTIFFIOHandler::canRead() const
{
    if(!device())
        return false;

    TIFF * tiff = tiffOpen(device());

    if (!tiff) {
        qWarning() << Q_FUNC_INFO << "can't open tiff file";
        return false;
    }

    TIFFClose(tiff);
    return true;
}

bool MultiTIFFIOHandler::read(QImage * image)
{
    if(!image)
        return false;

    TIFF * tiff = tiffOpen(device());

    if(!tiff) {
        qWarning() << Q_FUNC_INFO << "can't open tiff file";
        return false;
    }

    if(!TIFFSetDirectory(tiff, image_num_)) {
        qWarning() << Q_FUNC_INFO << "can't open tiff directory:" << image_num_;
        TIFFClose(tiff);
        return false;
    }

    // TODO: handle page numbers
    quint32 width = 0;
    quint32 height = 0;
    TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
    TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);

    if(!width || !height) {
        qWarning() << Q_FUNC_INFO << "invalid image dimensions";
        TIFFClose(tiff);
        return false;
    }

    uint32 * raster = (uint32*) _TIFFmalloc(width * height * sizeof (uint32));

    if(!raster) {
        TIFFClose(tiff);
        return false;
    }

    if(!TIFFReadRGBAImage(tiff, width, height, raster, 0))  {
        qWarning() << Q_FUNC_INFO << "can't read tiff RGBA image";
        _TIFFfree(raster);
        TIFFClose(tiff);
        return false;
    }

    QImage res(width, height, QImage::Format_ARGB32);

    for (quint32 row = 0; row < height; row++) {
        quint32 * dest_line = (quint32*) res.scanLine(height - row - 1);
        if(!dest_line) {
            qWarning() << Q_FUNC_INFO << "invalid scanline pointer" << height - row - 1;
            break;
        }

        for (quint32 col = 0; col < width; col++)
            dest_line[col] = raster[width * row + col];
    }


    _TIFFfree(raster);
    TIFFClose(tiff);
    *image = res;
    return true;
}

bool MultiTIFFIOHandler::jumpToImage(int imageNumber)
{
    image_num_ = 0;

    int counter = 0;
    while(counter < imageNumber) {
        if(!jumpToNextImage())
            return false;

        counter++;
    }

    return true;
}

bool MultiTIFFIOHandler::jumpToNextImage()
{
    if(!canRead())
        return 0;

    TIFF * tiff = tiffOpen(device());

    if (!tiff) {
        qWarning() << Q_FUNC_INFO << "can't open tiff image";
        return 0;
    }

    uint16 dircount = TIFFNumberOfDirectories(tiff);

    for(uint16 i = image_num_ + 1; i < dircount; i++) {
        if(!TIFFSetDirectory(tiff, i)){
            qWarning() << Q_FUNC_INFO << "can't set tiff directory:" << i;
            return false;
        }

        uint16 page_number = 0;
        uint16 page_total = 0;

        if(TIFFGetField(tiff, TIFFTAG_PAGENUMBER, &page_number, &page_total)) {
            image_num_ = i;
            TIFFClose(tiff);
            return true;
        }

        uint32 subfiletype = 0;

        // new subfiletype
        if(TIFFGetField(tiff, TIFFTAG_SUBFILETYPE, &subfiletype)) {
            if(((subfiletype & FILETYPE_PAGE) != 0) || (subfiletype == 0)) {
                image_num_ = i;
                TIFFClose(tiff);
                return true;
            }

            continue;
        }

        // old subfiletype
        if(TIFFGetField(tiff, TIFFTAG_OSUBFILETYPE, &subfiletype)) {
            if((subfiletype == OFILETYPE_IMAGE)
                || (subfiletype == OFILETYPE_PAGE)
                || (subfiletype == 0)) {
                image_num_ = i;
                TIFFClose(tiff);
                return true;
            }

            continue;
        }

        image_num_ = i;
        TIFFClose(tiff);
        return true;
    }

    TIFFClose(tiff);
    return false;
}

int MultiTIFFIOHandler::imageCount() const
{
    if(!canRead())
        return 0;

    return tiffImageCount(device());
}

QVariant MultiTIFFIOHandler::option(QImageIOHandler::ImageOption option) const
{
    switch(option) {
    case Size: {
        TIFF * tiff = tiffOpen(device());

        if (!tiff)
            return QVariant();

        quint32 width = 0;
        quint32 height = 0;

        if(!TIFFSetDirectory(tiff, image_num_) != 1) {
            qWarning() << Q_FUNC_INFO << "can't open tiff directory:" << image_num_;
            TIFFClose(tiff);
            return QVariant();
        }

        if(TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width) != 1) {
            qWarning() << Q_FUNC_INFO << "can't get tiff width";
            TIFFClose(tiff);
            return QVariant();
        }

        if(TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height) != 1) {
            qWarning() << Q_FUNC_INFO << "can't get tiff height";
            TIFFClose(tiff);
            return QVariant();
        }

        TIFFClose(tiff);

        return QSize(width, height);
    }
    default:
        return QImageIOHandler::option(option);
    }
}

bool MultiTIFFIOHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    switch(option) {
    case ImageFormat:
    case Size:
        return true;
    default:
        return QImageIOHandler::supportsOption(option);
    }
}
