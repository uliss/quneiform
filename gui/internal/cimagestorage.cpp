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

#include <QDebug>

#include "cimagestorage.h"
#include "cimage/ctiimage.h"
#include "cimage/cticontrol.h"
#include "rdib/ctdib.h"
#include "compat_defs.h"

CImageStorage::CImageStorage()
{
}

bool CImageStorage::hasImage(const QString& name) const
{
    return cf::CImage::instance().image(name.toStdString()) != NULL;
}

QPixmap CImageStorage::pixmap(const QString& name) const
{
    std::string n = name.toStdString();
    qDebug() << Q_FUNC_INFO << n.c_str();
    BitmapHandle h = cf::CImage::instance().image(n);

    if(!h) {
        qDebug() << Q_FUNC_INFO << "image not found: " << name;
        return QPixmap();
    }

    CTDIB dib;
    dib.SetDIBbyPtr(h);

    BITMAPFILEHEADER bf;
    memset(&bf, 0, sizeof(BITMAPFILEHEADER));
    bf.bfType = 0x4d42; // 'BM'
    bf.bfSize = sizeof(BITMAPFILEHEADER) + dib.GetDIBSize();
    // fileheader + infoheader + palette
    bf.bfOffBits = sizeof(BITMAPFILEHEADER) + dib.GetHeaderSize() + dib.GetRGBPalleteSize();

    uchar * buffer = (uchar*) malloc(bf.bfSize);
    memcpy(buffer, &bf, sizeof(bf));
    memcpy(buffer + sizeof(bf), h, dib.GetDIBSize());

    QPixmap res;
    res.loadFromData(buffer, sizeof(buffer), "BMP");

    free(buffer);
    return res;
}

QStringList CImageStorage::images() const
{
    QStringList res;

    typedef std::list<std::string> StringList;
    typedef StringList::iterator iterator;
    StringList names = cf::CImage::instance().images();
    for(iterator it = names.begin(); it != names.end(); ++it) {
        res.append(QString::fromStdString(*it));
    }

    return res;
}
