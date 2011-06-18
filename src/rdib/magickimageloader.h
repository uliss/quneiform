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

#ifndef MAGICKIMAGELOADER_H_
#define MAGICKIMAGELOADER_H_

#include "imageloader.h"
#include "common/image.h"

namespace Magick
{
class Image;
class Blob;
}

namespace cf
{

class MagickImageLoader: public ImageLoader
{
    public:
        MagickImageLoader();

        /**
         * Loads image from file
         * @return pointer to loaded image
         * @note caller have to delete return pointer
         * @throw ImageLoader::Exception on errors
         */
        ImagePtr load(const std::string& filename);

        /**
         * Loads image from stream
         * @return pointer to loaded image
         * @note caller have to delete return pointer
         * @throw ImageLoader::Exception on errors
         */
        ImagePtr load(std::istream& stream);
    private:
        void convertColorSpace(Magick::Image& image);
        void convertImageToDib(Magick::Image& image, Magick::Blob& blob);
        void convertImageDpi(Magick::Image& image);
        ImagePtr load(Magick::Image& image);
        ImagePtr makeImage(Magick::Blob& blob);
};

}

#endif /* MAGICKIMAGELOADER_H_ */
