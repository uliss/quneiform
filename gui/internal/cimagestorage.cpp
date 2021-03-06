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
#include <QMutexLocker>

#include "cimagestorage.h"
#include "cimage/ctiimage.h"
#include "cimage/cticontrol.h"
#include "common/ctdib.h"
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
    QMutexLocker locker(&lock_);

    std::string n = name.toStdString();
    cf::BitmapPtr h = cf::CImage::instance().image(n);

    if(!h) {
        qDebug() << Q_FUNC_INFO << "image not found: " << name;
        return QPixmap();
    }

    cf::CTDIB dib;
    dib.setBitmap(h);

    QPixmap res;
    if(!res.loadFromData((uchar*) h, dib.dibSize(), "DIB"))
        qWarning() << Q_FUNC_INFO << "can't load DIB:"  << name;

    return res;
}

QStringList CImageStorage::images() const
{
    QStringList res;

    typedef std::list<std::string> StringList;
    typedef StringList::iterator iterator;
    StringList names = cf::CImage::instance().images();
    for(iterator it = names.begin(); it != names.end(); ++it) {
        if((*it) != "ImageFromUser")
            res.append(QString::fromStdString(*it));
    }

    return res;
}
