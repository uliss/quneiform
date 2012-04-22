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

#ifndef CIMAGESTORAGE_H
#define CIMAGESTORAGE_H

#include <QPixmap>
#include <QStringList>

class CImageStorage
{
public:
    CImageStorage();

    /**
      * Returns true if image with given name exists in container
      */
    bool hasImage(const QString& name) const;

    /**
      * Returns image by given name
      */
    QPixmap pixmap(const QString& name) const;

    /**
      * Returns image names in storage
      */
    QStringList images() const;
};

#endif // CIMAGESTORAGE_H
