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

#ifndef IMAGEURL_H
#define IMAGEURL_H

#include <QString>

class ImageURL
{
public:
    explicit ImageURL(const QString& path, int imageNumber = 0);
    bool exists() const;
    bool isEmpty() const;
    bool isSimple() const;
    QString path() const;
    int imageNumber() const;
    QString id() const;

    void setImageNumber(int n);
    void setPath(const QString& path);
private:
    friend QDataStream& operator<<(QDataStream& stream, const ImageURL& imageURL);
    friend QDataStream& operator>>(QDataStream& stream, ImageURL& imageURL);

    QString path_;
    qint32 image_number_;
};

QDebug& operator<<(QDebug& dbg, const ImageURL& url);
QDataStream& operator<<(QDataStream& stream, const ImageURL& imageURL);
QDataStream& operator>>(QDataStream& stream, ImageURL& imageURL);

#endif // IMAGEURL_H
