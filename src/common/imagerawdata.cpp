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

#include <string.h>

#include "imagerawdata.h"

namespace cf
{

ImageRawData::ImageRawData() :
    data_(NULL), allocator_(AllocatorNone), data_size_(0) {
}

ImageRawData::ImageRawData(const ImageRawData& data) :
    data_(NULL),
    allocator_(AllocatorNone),
    data_size_(0)
{
    if(!data.isNull()) {
        data_size_ = data.data_size_;
        allocator_ = AllocatorNew;
        data_ = new uchar[data.data_size_];
        memcpy(data_, data.data_, data_size_);
    }
}

ImageRawData::ImageRawData(unsigned char * data, size_t size, allocator_t allocator) :
    data_(data), allocator_(allocator), data_size_(size) {

}

ImageRawData::~ImageRawData() {
    clear();
}

void ImageRawData::clear() {
    switch (allocator_) {
    case AllocatorMalloc:
        free(data_);
        break;
    case AllocatorNew:
        delete[] data_;
        break;
    default:
        ;
        // nothing to do
    }

    data_ = NULL;
    data_size_ = 0;
}

ImageRawData * ImageRawData::clone() const
{
    return new ImageRawData(*this);
}

unsigned char * ImageRawData::data() const {
    return data_;
}

size_t ImageRawData::dataSize() const {
    return data_size_;
}

void ImageRawData::set(unsigned char * data, size_t size, allocator_t allocator) {
    clear();

    data_ = data;
    data_size_ = size;
    allocator_ = allocator;
}

}

std::ostream& operator<<(std::ostream& os, const cf::ImageRawData& image) {
    os.write((char*) image.data(), static_cast<std::streamsize>(image.dataSize()));
    return os;
}
