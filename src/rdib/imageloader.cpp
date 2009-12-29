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

#include "imageloader.h"

namespace CIF
{

ImageLoader::ImageLoader() :
    max_image_size_(1024 * 1024 * 40)
{
}

ImageLoader::~ImageLoader()
{
}

bool ImageLoader::isValidImageSize(size_t size) const
{
    return size < max_image_size_;
}

void ImageLoader::setMaxImageSize(size_t size)
{
    max_image_size_ = size;
}

size_t ImageLoader::streamSize(std::istream& is)
{
    size_t prev_pos = is.tellg();
    is.seekg(0, std::ios::end);
    size_t ret = is.tellg();
    is.seekg(prev_pos);
    return ret;
}

}
