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

#include <boost/shared_ptr.hpp>
#include <iostream>
#include "globus.h"
#include "serialize.h"

#ifdef CF_SERIALIZE
#include <boost/serialization/binary_object.hpp>
#endif

namespace cf
{

class CLA_EXPO ImageRawData
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
         * Returns pointer to raw data deep copy
         */
        virtual ImageRawData * clone() const;

        /**
         * Returns pointer to image data
         * @see dataSize()
         */
        unsigned char * data() const;

        /**
         * Returns image data size
         * @see data()
         */
        size_t dataSize() const;

        /**
         * Checks if image data empty
         */
        bool isNull() const;

        /**
         * Sets image raw data. Previous data is cleared
         */
        void set(unsigned char * data, size_t size, allocator_t allocator);
    protected:
        ImageRawData(const ImageRawData& data);
    private:
        void operator=(const ImageRawData&);
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;

        template<class Archive>
        void save(Archive& a, const unsigned /*version*/) const
        {
            using boost::serialization::make_nvp;
            a << make_nvp("data_size", data_size_);
            a << make_nvp("data", boost::serialization::make_binary_object(data_, data_size_ * sizeof(unsigned char)));
        }

        template<class Archive>
        void load(Archive& a, const unsigned /*version*/)
        {
            using boost::serialization::make_nvp;
            a >> make_nvp("data_size", data_size_);
            // allocating memory
            data_ = new uchar[data_size_];
            allocator_ = AllocatorNew;
            a >> make_nvp("data", boost::serialization::make_binary_object(data_, data_size_));
        }

        BOOST_SERIALIZATION_SPLIT_MEMBER()
#endif
        unsigned char * data_;
        allocator_t allocator_;
        size_t data_size_;
};

inline bool ImageRawData::isNull() const {
    return data_ == NULL || data_size_ == 0;
}

typedef boost::shared_ptr<ImageRawData> ImageRawPtr;

}
FUN_EXPO__ std::ostream& operator<<(std::ostream& os, const cf::ImageRawData& image);

#endif /* IMAGERAWDATA_H_ */
