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

#include <QDataStream>

#include "imageurl.h"

ImageURL::ImageURL(const QString& path, int imageNumber) :
    path_(path),
    image_number_(imageNumber)
{
}

bool ImageURL::isEmpty() const
{
    return path_.isEmpty();
}

bool ImageURL::isSimple() const
{
    return image_number_ == 0;
}

QString ImageURL::path() const
{
    return path_;
}

int ImageURL::imageNumber() const
{
    return image_number_;
}

QString ImageURL::id() const
{
    QString res = path_;
    if(!isSimple())
        res += QString("%1").arg(image_number_);

    return res;
}

void ImageURL::setImageNumber(int n)
{
    image_number_ = n;
}

void ImageURL::setPath(const QString& path)
{
    path_ = path;
}

QDataStream& operator<<(QDataStream& os, const ImageURL& imageURL)
{
    os << imageURL.path_
       << imageURL.image_number_;
    return os;
}

QDataStream& operator>>(QDataStream& is, ImageURL& url)
{
    is >> url.path_
       >> url.image_number_;
    return is;
}
