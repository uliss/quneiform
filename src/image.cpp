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
#include "image.h"
#include <string.h>
#include <iostream>
#include <Magick++.h>

using namespace std;

namespace CIF
{

Image::Image(size_t ht, size_t wd) : m_blob(0)
{
    setHeight(ht);
    setWidth(wd);
}

Image::~Image()
{
    if (m_blob != 0)
        delete [](char*)m_blob;
}

bool Image::allocate(size_t sz)
{
    if (m_blob != 0)
        delete [](char*)m_blob;
    m_blob = new char[sz];
    // ???
    return m_blob == 0 ? false : true;
}

void * Image::data()
{
    return m_blob;
}

void Image::exportTo(const Exporter& exp) const
{
    exp.exportImage(*this);
}


size_t Image::height() const
{
    return m_height;
}

void Image::init(const void * ib, size_t sz)
{
    allocate(sz);
    memcpy(m_blob, ib, sz);
    m_size = sz;
}

void Image::load(const string& fname)
{
    Magick::Blob blob;
    try {
        Magick::Image image(fname);
        // Write to BLOB in BMP format
        image.write(&blob, "PNG");
        setHeight(image.rows());
        setWidth(image.columns());
    }
    catch (Magick::Exception &error) {
        std::cerr << error.what() << "\n";
    }

    init(blob.data(), blob.length());
}

void Image::save(const std::string& fname) const
{
    if (m_blob == 0) return;
    try {
        Magick::Blob blob(m_blob, m_size);
        Magick::Image image;
        image.magick("PNG");
        image.read(blob);
        image.write(fname);
    }
    catch (Magick::Exception &error) {
        std::cerr << error.what() << "\n";
    }
}

void Image::setHeight(const size_t& value)
{
    m_height = value;
}

size_t Image::width() const
{
    return m_width;
}

void Image::setWidth(const size_t& value)
{
    m_width = value;
}

}
