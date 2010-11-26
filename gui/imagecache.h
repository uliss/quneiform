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

#ifndef IMAGECACHE_H
#define IMAGECACHE_H

class QString;
class QPixmap;

class ImageCache
{
    ImageCache();
public:
    /**
      * Checks is pixmap exists in cache
      */
    static bool find(const QString& path, QPixmap * pixmap);

    /**
      * Inserts pixmap into cache
      */
    static bool insert(const QString& path, const QPixmap& pixmap);

    /**
      * Loads pixmap from cache, if not found load from file
      * otherwise returns false
      */
    static bool load(const QString& path, QPixmap * pixmap);
};

#endif // IMAGECACHE_H
