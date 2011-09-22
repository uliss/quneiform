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

#ifndef SHAREDIMAGE_H
#define SHAREDIMAGE_H

#include <cstddef>
#include <string>

#include "globus.h"
#include "common/image.h"
#include "common/size.h"

namespace cf {

class CLA_EXPO SharedImage
{
public:
    SharedImage();
    SharedImage(ImagePtr img);

    /**
      * Zeroes image memory, sets dimensions to Size(0, 0) and
      * sets name to empty string
      */
    void clear();

    /**
      * Returns pointer to raw image data
      */
    void * data();

    /**
      * Returns shared image data size
      */
    size_t dataSize() const;

    /**
      * Returns shared image dimensions
      */
    Size dimensions() const;

    /**
      * Returns image pointer constructed from shared image
      * @see set()
      */
    ImagePtr get();

    /**
      * Returns image name
      */
    std::string name() const;

    /**
      * Sets shared image content from given iamge pointer
      * @see get()
      */
    void set(ImagePtr img);
private:
    void copyData(ImagePtr img);
    void copyInfo(ImagePtr img);
private:
    static const size_t MAX_SIZE = 1024 * 1024 * 50;
    static const size_t MAX_NAME = 1024;
private:
    unsigned char data_[MAX_SIZE];
    char name_[MAX_NAME];
    Size dimensions_;
    size_t data_size_;
};

inline void * SharedImage::data() {
    return data_;
}

}

#endif // SHAREDIMAGE_H
