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

#ifndef CEDPICTURE_H_
#define CEDPICTURE_H_

#include "globus.h"
#include "cedchar.h"
#include "common/image.h"

namespace CIF
{

class CLA_EXPO CEDPicture: public CEDChar
{
    public:
        CEDPicture();

        /**
         * Returns picture alignment
         * @see setAlignment()
         */
        ed_align_t alignment() const;

        /**
         * Returns pointer to image
         * @see setImage()
         */
        ImagePtr image();

        /**
         * Returns true
         */
        bool isPicture() const;

        /**
         * Returns picture number
         */
        int pictureNumber() const;

        /**
         * Sets picture align
         * @see alignment()
         */
        void setAlignment(ed_align_t align);

        /**
         * Sets image
         * @see image
         */
        void setImage(ImagePtr image);

        /**
         * Sets picture number
         * @see pictureNumber()
         */
        void setPictureNumber(int number);
    private:
        ed_align_t align_;
        int number_;
        ImagePtr image_;
        Size goal_;
};

}

#endif /* CEDPICTURE_H_ */
