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

namespace CIF
{

class Image: public boost::noncopyable
{
    public:
        enum allocator_t
        {
            AllocatorMalloc,
            AllocatorNew
        };

        Image(char * src, size_t size, allocator_t allocator = AllocatorMalloc);
        virtual ~Image();
        char * data();
        void setData(char * src, size_t size, allocator_t allocator = AllocatorMalloc);
        size_t size() const;
    private:
        void clear();
    private:
        char * data_;
        size_t size_;
        allocator_t allocator_;
};

typedef boost::shared_ptr<Image> ImagePtr;

}

#endif /* IMAGE_H_ */
