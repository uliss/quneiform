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
#include "element.h"
#include "ced.h"
#include "common/image.h"
#include "common/serialize.h"

#ifdef CF_SERIALIZE
#include <boost/serialization/shared_ptr.hpp>
#endif

namespace CIF
{

class CLA_EXPO CEDPicture: public Element
{
    public:
        CEDPicture();

        /**
         * Returns picture alignment
         * @see setAlignment()
         */
        ed_align_t alignment() const;

        void exportElement(CEDExporter& exp);

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
        void setImage(Image * image);

        /**
         * Sets picture number
         * @see pictureNumber()
         */
        void setPictureNumber(int number);
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            ar & boost::serialization::base_object<Element>(*this);
            ar & align_;
            ar & number_;
            ar & image_;
            ar & goal_;
        }
#endif
    private:
        ed_align_t align_;
        int number_;
        ImagePtr image_;
        Size goal_;
};

}

#endif /* CEDPICTURE_H_ */
