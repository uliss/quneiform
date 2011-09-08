/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#include <algorithm>
#include <iostream>
#include <boost/current_function.hpp>

#include "sharedimage.h"
#include "common/tostring.h"
#include "common/console_messages.h"
#include "common/debug.h"

#define CF_INFO cf::Debug() << cf::console::info << BOOST_CURRENT_FUNCTION
#define CF_ERROR std::cerr << cf::console::error << BOOST_CURRENT_FUNCTION

namespace cf {

SharedImage::SharedImage() :
    data_size_(0)
{
    name_[0] = 0;
}

SharedImage::SharedImage(ImagePtr img)
{
    set(img);
}

void SharedImage::clear() {
    data_size_ = 0;
    std::fill(data_, data_ + MAX_SIZE, '\0');
    name_[0] = '\0';
    dimensions_ = Size();
}

void SharedImage::copyData(ImagePtr img) {
    assert(img);

    data_size_ = img->dataSize();
    std::copy(img->data(), img->data() + img->dataSize(), data_);

    CF_INFO << " data size: " << data_size_ << " bytes\n";
}

void SharedImage::copyInfo(ImagePtr img) {
    assert(img);
    dimensions_ = img->size();

    std::string fname = img->fileName();

    if(fname.size() < MAX_NAME) {
        std::copy(fname.begin(), fname.end(), name_);
        name_[fname.size()] = 0;
    }
    else {
        CF_ERROR << " file name is too big: " << img->fileName() << "\n";
    }
}

size_t SharedImage::dataSize() const {
    return data_size_;
}

Size SharedImage::dimensions() const {
    return dimensions_;
}

ImagePtr SharedImage::get() {
    if(data_size_ == 0)
        return ImagePtr();

    ImagePtr res(new Image(data_, data_size_, ImageRawData::AllocatorNone));
    res->setSize(dimensions_);
    res->setFileName(name_);
    return res;
}

std::string SharedImage::name() const {
    return std::string(name_);
}

void SharedImage::set(ImagePtr img) {
    if(!img) {
        clear();
        return;
    }

    if(MAX_SIZE < img->dataSize()) {
        CF_ERROR << " given image is too big: " << img->dataSize() << " bytes\n";
        return;
    }

    copyData(img);
    copyInfo(img);
}

}
