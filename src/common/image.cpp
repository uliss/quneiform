/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#include <cstdlib>
#include <cstring>
#include <cassert>

#include "image.h"

namespace cf
{

Image::Image() {
}

Image::Image(uchar * src, size_t size, allocator_t allocator) :
    ImageRawData(src, size, allocator) {
}

Image::Image(const Image& image) :
    ImageRawData(image),
    fname_(image.fname_),
    size_(image.size_)
{}

Image * Image::clone() const
{
    return new Image(*this);
}

std::string Image::fileName() const {
    return fname_;
}

int Image::height() const {
    return size_.height();
}

void Image::setFileName(const std::string& fname) {
    fname_ = fname;
}

void Image::setSize(const Size& size) {
    size_ = size;
}

Size Image::size() const {
    return size_;
}

int Image::width() const {
    return size_.width();
}

}
