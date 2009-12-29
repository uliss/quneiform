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

namespace CIF
{

Image::Image(char * src, size_t size, allocator_t allocator) :
    data_(src), size_(size), allocator_(allocator)
{
}

Image::~Image()
{
    clear();
}

void Image::clear()
{
    if (allocator_ == AllocatorMalloc)
        free(data_);
    else
        delete[] data_;
    data_ = NULL;
    size_ = 0;
}

char * Image::data()
{
    return data_;
}

size_t Image::size() const
{
    return size_;
}

void Image::setData(char * src, size_t size, allocator_t allocator)
{
    clear();
    allocator_ = allocator;
    data_ = src;
    size_ = size;
}

}
