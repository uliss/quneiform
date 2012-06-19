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

#ifndef IMAGE_H_
#define IMAGE_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

#include "globus.h"
#include "common/imagerawdata.h"
#include "common/size.h"
#include "common/dib.h"
#include "common/serialize.h"

#ifdef CF_SERIALIZE
#include <boost/serialization/shared_ptr.hpp>
#endif

namespace cf
{

class CLA_EXPO Image: public ImageRawData
{
    public:
        Image();
        Image(uchar * src, size_t size, allocator_t allocator);

        /**
         * Returns image filename
         */
        std::string fileName() const;

        /**
         * Returns image height
         * @see size(), width()
         */
        int height() const;

        /**
         * Sets image filename
         */
        void setFileName(const std::string& fname);

        /**
         * Sets image size
         * @see size()
         */
        void setSize(const Size& size);

        /**
         * Returns image size
         * @see height(), width()
         */
        Size size() const;

        /**
         * Returns image width
         * @see size()
         */
        int width() const;
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int /*version*/) {
            using boost::serialization::make_nvp;
            ar & make_nvp("raw", boost::serialization::base_object<ImageRawData>(*this));
            ar & make_nvp("filename", fname_);
            ar & make_nvp("size", size_);
        }
#endif
    private:
        std::string fname_;
        Size size_;
};

typedef boost::shared_ptr<Image> ImagePtr;

}

#endif /* IMAGE_H_ */
