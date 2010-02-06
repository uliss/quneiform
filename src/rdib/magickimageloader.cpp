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

bool unkwnown = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_UNKNOWN, 100,
        create);
bool bmp = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_BMP, 100, create);
bool gif = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_GIF, 100, create);
bool jpg = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_JPEG, 100, create);
bool pnm = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_PNM, 100, create);
bool png = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_PNG, 100, create);
bool tif = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_TIFF, 100, create);
bool xpm = CIF::ImageLoaderFactory::instance().registerCreator(CIF::FORMAT_XPM, 100, create);
}

namespace CIF
{

MagickImageLoader::MagickImageLoader()
{
}

MagickImageLoader::~MagickImageLoader()
{
}

ImagePtr MagickImageLoader::load(Magick::Image& image, Magick::Blob& blob)
{
    switch (image.type()) {
    case Magick::BilevelType:
    case Magick::TrueColorType:
        break;
    default:
        image.type(Magick::TrueColorType);
    }

    if (CIF::Config::instance().debugHigh())
        image.verbose(true);
    // Write to BLOB in BMP format

    image.magick("DIB");
    image.write(&blob);

    char * new_data = new char[blob.length()];
    memcpy(new_data, blob.data(), blob.length());
    return ImagePtr(new Image(new_data, blob.length(), Image::AllocatorNew));
}

ImagePtr MagickImageLoader::load(std::istream& stream)
{
    if(stream.fail())
        throw Exception("MagickImageLoader::load invalid stream given");

    std::ios::streampos stream_size = streamSize(stream);
    if(stream_size < 1)
        throw Exception("MagickImageLoader::load empty stream given");

    boost::scoped_array<char> tmp(new char[stream_size]);
    Magick::Blob blob;
    blob.updateNoCopy(tmp.get(), static_cast<size_t>(stream_size));
    try {
        Magick::Image image(blob);
        return load(image, blob);
    }
    catch (Magick::Exception &e) {
        std::cerr << e.what() << "\n";
        throw Exception("MagickImageLoader::load faild");
    }
}

ImagePtr MagickImageLoader::load(const std::string& fname)
{
    Magick::Blob blob;
    try {
        Magick::Image image(fname);
        return load(image, blob);
    }
    catch (Magick::Exception &e) {
        std::cerr << e.what() << "\n";
        throw Exception("MagickImageLoader::load faild");
    }
}

}

