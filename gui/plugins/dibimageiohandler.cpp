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

#include <QImage>
#include <QDebug>
#include <string.h>

#include "dibimageiohandler.h"
#include "cfcompat.h"
#include "rdib/ctdib.h"

static const int VERSION_3_HEADER_SIZE = 40;
static const int VERSION_4_HEADER_SIZE = 108;
static const int VERSION_5_HEADER_SIZE = 124;

static bool readDIBHeader(QIODevice * device, BITMAPINFOHEADER& info)
{
    if(!device)
        return false;

    qint64 read = device->peek((char*) &info, sizeof(BITMAPINFOHEADER));
    if(read != sizeof(BITMAPINFOHEADER))
        return false;

    return true;
}

static bool isValidDIBHeader(BITMAPINFOHEADER& h)
{
    if(h.biSize != VERSION_3_HEADER_SIZE &&
            h.biSize != VERSION_4_HEADER_SIZE &&
            h.biSize != VERSION_5_HEADER_SIZE) {
        qDebug() <<Q_FUNC_INFO << "unknown dib header size:" << h.biSize;
        return false;
    }

    if(h.biPlanes != 1)
        return false;

    if(h.biBitCount != 0 &&
            h.biBitCount != 1 &&
            h.biBitCount != 4 &&
            h.biBitCount != 8 &&
            h.biBitCount != 16 &&
            h.biBitCount != 24 &&
            h.biBitCount != 32)
        return false;

    return true;
}

static void writeBMPHeader(BITMAPFILEHEADER * bmp, BITMAPINFOHEADER * info, char * data)
{
    if(!bmp || !info)
        return;

    CTDIB dib;
    dib.SetDIBbyPtr(info);

    const size_t bitmap_size = dib.GetDIBSize();
    const size_t bmp_header_size = sizeof(BITMAPFILEHEADER);

    memset(bmp, 0, bmp_header_size);
    bmp->bfType = 0x4d42; // 'BM'
    bmp->bfSize = (uint32_t) (bmp_header_size + bitmap_size);
    // fileheader + infoheader + palette
    bmp->bfOffBits = bmp_header_size +
            dib.GetHeaderSize() +
            dib.GetRGBPalleteSize();

//    memcpy(d, bmp, bmp_header_size);
}

DIBImageIOHandler::DIBImageIOHandler()
{
    qDebug() << Q_FUNC_INFO;
}

bool DIBImageIOHandler::canRead() const
{
    BITMAPINFOHEADER bitmap_info;
    if(!readDIBHeader(device(), bitmap_info))
        return false;

    return isValidDIBHeader(bitmap_info);
}

bool DIBImageIOHandler::read(QImage * image)
{
    BITMAPINFOHEADER bitmap_info;
    if(!readDIBHeader(device(), bitmap_info))
        return false;

    if(!isValidDIBHeader(bitmap_info)) {
        qDebug() << Q_FUNC_INFO << "invalid header";
        return false;
    }

    CTDIB dib;
    dib.SetDIBbyPtr(&bitmap_info);
    const size_t bitmap_size = dib.GetDIBSize();
    const size_t bmp_header_size = sizeof(BITMAPFILEHEADER);

    BITMAPFILEHEADER bmp_header;
    memset(&bmp_header, 0, bmp_header_size);
    bmp_header.bfType = 0x4d42; // 'BM'
    bmp_header.bfSize = (uint32_t) (bmp_header_size + bitmap_size);
    // fileheader + infoheader + palette
    bmp_header.bfOffBits = bmp_header_size +
            dib.GetHeaderSize() +
            dib.GetRGBPalleteSize();

    char * buf = new char[bmp_header.bfSize];
    char * buf_ptr = buf;
    memcpy(buf_ptr, &bmp_header, bmp_header_size);
    buf_ptr += bmp_header_size;

    if(!device()->read(buf_ptr, bitmap_size)) {
        delete[] buf;
        qDebug() << Q_FUNC_INFO << "read error";
        return false;
    }

    bool rc = image->loadFromData((uchar*) buf, (int) bmp_header.bfSize, "BMP");
    delete[] buf;
    return rc;
}

bool DIBImageIOHandler::write(const QImage&)
{
    return false;
}

QVariant DIBImageIOHandler::option(QImageIOHandler::ImageOption option) const
{
    switch(option) {
    case Size: {
        BITMAPINFOHEADER dib_info;
        if(!readDIBHeader(device(), dib_info))
            return QVariant();

        return QSize(qAbs(dib_info.biWidth), qAbs(dib_info.biHeight));
    }
    default:
        return QImageIOHandler::option(option);
    }
}

bool DIBImageIOHandler::supportsOption(QImageIOHandler::ImageOption option) const
{
    switch(option) {
    case Size:
        return true;
    default:
        return QImageIOHandler::supportsOption(option);
    }
}
