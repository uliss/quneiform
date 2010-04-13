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

#ifndef IMAGERAWDATA_H_
#define IMAGERAWDATA_H_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include "globus.h"
#include "serialize.h"

#ifdef CF_SERIALIZE
#include <boost/serialization/binary_object.hpp>
#endif

namespace CIF
{

class CLA_EXPO ImageRawData: public boost::noncopyable
{
    public:
        enum allocator_t
        {
            AllocatorNone, AllocatorMalloc, AllocatorNew
        };

        ImageRawData();
        ImageRawData(unsigned char * data, size_t size, allocator_t allocator);

        virtual ~ImageRawData();

        /**
         * Deletes image raw data
         */
        void clear();

        /**
         * Returns pointer to image data
         */
        unsigned char * data() const;

        /**
         * Checks if image data empty
         */
        bool isNull() const;

        /**
         * Sets image raw data. Previous data is cleared
         */
        void set(unsigned char * data, size_t size, allocator_t allocator);

        /**
         * Returns image data size
         */
        size_t size() const;
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;

        template<class Archive>
        void save(Archive& a, const unsigned /*version*/) const
        {
            a << size_;
            a << boost::serialization::make_binary_object(data_, size_ * sizeof(unsigned char));
        }

        template<class Archive>
        void load(Archive& a, const unsigned /*version*/)
        {
            a >> size_;
            // allocating memory
            data_ = new uchar[size_];
            a.load_binary(data_, size_);
            allocator_ = AllocatorNew;
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif
        unsigned char * data_;
        allocator_t allocator_;
        size_t size_;
};

std::ostream& operator<<(std::ostream& os, const ImageRawData& image);

inline bool ImageRawData::isNull() const {
    return data_ == NULL || size_ == 0;
}

typedef boost::shared_ptr<ImageRawData> ImageRawPtr;

}

#endif /* IMAGERAWDATA_H_ */
