/***************************************************************************
 *   Copyright (C) 2008 by Сергей Полтавский   *
 *   poltava@gentoo   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef CIFIMAGE_H
#define CIFIMAGE_H

#include "element.h"

namespace CIF
{

/**
 @author Serj Poltavskiy <serge.uliss at gmail.com>
*/
class Image : public Element
{
        size_t m_size;
        size_t m_height;
        size_t m_width;
        void * m_blob;
    public:
        Image(size_t height = 0, size_t width = 0);
        ~Image();
        bool allocate(size_t sz);
        void * data();
        void exportTo(const Exporter& exp) const;
        size_t height() const;
        void init(const void * ib, size_t sz);
        void load(const std::string& fname);
        void save(const std::string& fname) const;
        void setHeight(const size_t& value);
        void setWidth(const size_t& value);
        size_t width() const;
};

}

#endif
