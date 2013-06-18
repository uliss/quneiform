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

#ifndef QTIMAGELOADER_H_
#define QTIMAGELOADER_H_

#include "imageloader.h"
#include "globus.h"

class QImage;
class QString;

namespace cf
{

class CLA_EXPO QtImageLoader: public ImageLoader
{
public:
    QtImageLoader();

    /**
      * Loads image
      * @param url - image url
      * @return image pointer
      */
    ImagePtr load(const ImageURL& url);
    ImagePtr load(std::istream& is);

    /**
      * Loads image from QImage
      * @note caller should free pointer
      */
    static ImagePtr fromQImage(const QImage& image);

    /**
      * Returns list of supported formats
      */
    ImageFormatList supportedFormats() const;

    /**
     * Manually register image format loader callbacks
     * @note mainly for macosx
     */
    static void registerFormats();
private:
    ImagePtr loadPdf(const ImageURL& url);
    ImagePtr loadTiff(const ImageURL& url);
};

}

#endif /* QTIMAGELOADER_H_ */
