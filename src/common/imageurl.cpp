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

#include <iostream>

#include "imageurl.h"
#include "filesystem.h"
#include "helper.h"

namespace cf
{

ImageURL::ImageURL() :
    image_number_(0)
{
}

ImageURL::ImageURL(const std::string& path, int imageNumber) :
    path_(path),
    image_number_(imageNumber)
{
}

bool ImageURL::empty() const
{
    return path_.empty();
}

bool ImageURL::exists() const
{
    if(empty())
        return false;

    return fs::fileExists(path_);
}

std::string ImageURL::extension() const
{
    std::string res = fs::fileExtension(path_);
    toLower(res);
    return res;
}

int ImageURL::imageNumber() const
{
    return image_number_;
}

bool ImageURL::operator==(const ImageURL& url) const
{
    return path_ == url.path_ && image_number_ == url.image_number_;
}

std::string ImageURL::path() const
{
    return path_;
}

void ImageURL::setImageNumber(int number)
{
    image_number_ = number;
}

void ImageURL::setPath(const std::string& path)
{
    path_ = path;
}

bool ImageURL::simple() const
{
    return image_number_ == 0;
}

std::ostream& operator<<(std::ostream& os, const cf::ImageURL& url)
{
    os << url.path();
    if(!url.simple())
        os << "; image:" << url.imageNumber();

    return os;
}

}



