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

namespace CIF
{

class CLA_EXPO QtImageLoader : public ImageLoader
{
    public:
        QtImageLoader();
        ~QtImageLoader();

        /**
         * Returns pointer to loaded image
         * @return NULL if image not loaded
         */
        QImage * image();

        /**
         * Checks if image loaded
         */
        bool isLoaded() const;

        /**
         * Loads image
         * @param path - image path
         * @return image pointer
         */
        ImagePtr load(const std::string& path);
        ImagePtr load(std::istream& is);
    private:
        void clearImage();
    private:
        QImage * image_;

};

}

#endif /* QTIMAGELOADER_H_ */
