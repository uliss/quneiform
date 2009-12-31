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

#include <Magick++.h>
#include <cstring>
#include <boost/scoped_array.hpp>

#include "magickimageloader.h"
#include "imageloaderfactory.h"
#include "common/cifconfig.h"

namespace
{

CIF::ImageLoader * create()
{
    return new CIF::MagickImageLoader;
}

bool bmp = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_UNKNOWN, -1000, create);
}

namespace CIF
{

MagickImageLoader::MagickImageLoader()
{
}

MagickImageLoader::~MagickImageLoader()
{
}

Image * MagickImageLoader::load(Magick::Image * image, Magick::Blob * blob)
{
    assert(image);
    assert(blob);

    switch (image->type()) {
    case Magick::BilevelType:
    case Magick::TrueColorType:
        break;
    default:
        image->type(Magick::TrueColorType);
    }
    if (CIF::Config::instance().debugHigh())
        image->verbose(true);
    // Write to BLOB in BMP format
    image->magick("DIB");
    image->write(blob);

    char * new_data = new char[blob->length()];
    memcpy(new_data, blob->data(), blob->length());
    Image * ret = new Image(new_data, blob->length(), Image::AllocatorNew);
    return ret;
}

Image* MagickImageLoader::load(std::istream& stream)
{
    size_t stream_size = streamSize(stream);
    boost::scoped_array<char> tmp(new char[stream_size]);
    Magick::Blob blob;
    blob.updateNoCopy(tmp.get(), stream_size);
    try {
        Magick::Image image(blob);
        return load(&image, &blob);
    }
    catch (Magick::Exception &error_) {
        std::cerr << error_.what() << "\n";
        return NULL;
    }
}

Image* MagickImageLoader::load(const std::string& fname)
{
    Magick::Blob blob;
    try {
        Magick::Image image(fname);
        return load(&image, &blob);
    }
    catch (Magick::Exception &error_) {
        std::cerr << error_.what() << "\n";
        return NULL;
    }
    }

}

