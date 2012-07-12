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

#ifndef IMAGELOADER_H_
#define IMAGELOADER_H_

#include <vector>
#include <string>
#include <iostream>

#include "common/exception.h"
#include "common/image.h"
#include "common/imageformats.h"
#include "globus.h"

namespace cf
{

class Image;
class ImageURL;
typedef std::vector<image_format_t> ImageFormatList;

class CLA_EXPO ImageLoader
{
public:
    ImageLoader();
    virtual ~ImageLoader();

    bool isValidImageSize(size_t size) const;

    /**
      * Loads image from given url
      * @return image pointer
      */
    virtual ImagePtr load(const ImageURL& url) = 0;

    /**
      * Loads image from given data stream
      * @return image pointer
      */
    virtual ImagePtr load(std::istream& stream) = 0;

    void setMaxImageSize(size_t size);

    /**
      * Returns list of supported formats
      * @see supportsFormat()
      */
    virtual ImageFormatList supportedFormats() const;

    /**
     * Returns true if format supported
     * @see supportedFormats()
     */
    bool supportsFormat(image_format_t format) const;
public:
    /**
      * Returns size of stream or 0 if stream error occurs
      */
    static std::streampos streamSize(std::istream& stream);
public:
    typedef RuntimeExceptionImpl<ImageLoader> Exception;
private:
    size_t max_image_size_;
};

}

#endif /* IMAGELOADER_H_ */
