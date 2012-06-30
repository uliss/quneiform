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

#include <QStringList>
#include <QFileInfo>
#include <QImageReader>
#include <QDebug>

#include "imageutils.h"

namespace utils
{

static inline bool isQpfExt(const QString& ext)
{
    return ext == "qpf";
}

static inline bool isTiffExt(const QString& ext)
{
    return ext == "tif" || ext == "tiff";
}

static inline bool isPdfExt(const QString& ext)
{
    return ext == "pdf";
}

static inline QString fileExt(const QString& path)
{
    return QFileInfo(path).suffix().toLower();
}

static int multiPageImageCount(const QString& path)
{
    QByteArray format = imagePluginFormat(path);

    if(format.isEmpty())
        return 0;

    QImageReader r(path, format);

    if(!r.canRead())
        return 0;

    return r.imageCount();
}

int imageCount(const QStringList& files)
{
    int res = 0;
    foreach(QString path, files) {
        if(looksLikeMultiPageDocument(path))
            res += multiPageImageCount(path);
        else
            res++;
    }
    return res;
}

bool looksLikeMultiPageDocument(const QString& path)
{
    QString ext = fileExt(path);

    if(isTiffExt(ext) || isPdfExt(ext) || isQpfExt(ext))
        return true;

    return false;
}

const char * imagePluginFormat(const QString& path)
{
    QString ext = fileExt(path);

    if(isTiffExt(ext))
        return "MTIFF";

    if(isPdfExt(ext))
        return "PDF";

    return "";
}

}
