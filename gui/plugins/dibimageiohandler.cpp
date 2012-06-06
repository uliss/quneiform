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

DIBImageIOHandler::DIBImageIOHandler()
{
    qDebug() << Q_FUNC_INFO;
}

bool DIBImageIOHandler::canRead() const
{
    BITMAPINFOHEADER bitmap_info;
    qint64 read = device()->peek((char*) &bitmap_info, sizeof(BITMAPINFOHEADER));
    if(read != sizeof(BITMAPINFOHEADER))
        return false;

    return isValidDIBHeader(bitmap_info);
}

bool DIBImageIOHandler::read(QImage * image)
{
    BITMAPINFOHEADER bitmap_info;
    qint64 read = device()->peek((char*) &bitmap_info, sizeof(BITMAPINFOHEADER));
    if(read != sizeof(BITMAPINFOHEADER)) {
        qDebug() << Q_FUNC_INFO << "can't read header: only" << read << "bytes read";
        return false;
    }

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
    bmp_header.bfSize = bmp_header_size + bitmap_size;
    // fileheader + infoheader + palette
    bmp_header.bfOffBits = bmp_header_size +
            dib.GetHeaderSize() +
            dib.GetRGBPalleteSize();

    uchar * buf = new uchar[bmp_header.bfSize];
    uchar * buf_ptr = buf;
    memcpy(buf_ptr, &bmp_header, bmp_header_size);
    buf_ptr += bmp_header_size;

    QByteArray data = device()->read(bitmap_size);

    if(data.isEmpty()) {
        delete[] buf;
        return false;
    }

    memcpy(buf_ptr, data.constData(), bitmap_size);

    bool rc = image->loadFromData(buf, bmp_header.bfSize, "BMP");
    delete[] buf;
    return rc;
}

bool DIBImageIOHandler::write(const QImage& image)
{
    return false;
}
