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

#include <cassert>

#include "imageformatdetector.h"
#include "common/imageurl.h"
#include "common/debug.h"

namespace cf
{

ImageFormatDetector::ImageFormatDetector()
{
    addExt("bmp", FORMAT_BMP);
    addExt("gif", FORMAT_GIF);
    addExt("jpg", FORMAT_JPEG);
    addExt("jpeg", FORMAT_JPEG);
    addExt("png", FORMAT_PNG);
    addExt("pnm", FORMAT_PNM);
    addExt("pbm", FORMAT_PNM);
    addExt("pgm", FORMAT_PNM);
    addExt("ppm", FORMAT_PNM);
    addExt("tiff", FORMAT_TIFF);
    addExt("tif", FORMAT_TIFF);
    addExt("xpm", FORMAT_XPM);
    addExt("pdf", FORMAT_PDF);

    addMagick("BM", FORMAT_BMP);
    addMagick("GIF", FORMAT_GIF);
    addMagick("\xFF\xD8", FORMAT_JPEG);
    addMagick("\x89PNG", FORMAT_PNG);
    addMagick("II", FORMAT_TIFF);
    addMagick("MM", FORMAT_TIFF);
    addMagick("P1", FORMAT_PNM);
    addMagick("P2", FORMAT_PNM);
    addMagick("P3", FORMAT_PNM);
    addMagick("P4", FORMAT_PNM);
    addMagick("P5", FORMAT_PNM);
    addMagick("P6", FORMAT_PNM);
    addMagick("/* XPM */", FORMAT_XPM);
    addMagick("%PDF", FORMAT_PDF);
}

ImageFormatDetector& ImageFormatDetector::instance()
{
    static ImageFormatDetector instance;
    return instance;
}

image_format_t ImageFormatDetector::detect(const ImageURL& url) const
{
    FormatMap::const_iterator it = extension_format_map_.find(url.extension());
    return (it == extension_format_map_.end()) ? FORMAT_UNKNOWN : it->second;
}

image_format_t ImageFormatDetector::detect(std::istream& stream) const
{
    if (!stream.good()) {
        Debug() << "Bad stream given\n";
        return FORMAT_UNKNOWN;
    }

    static const unsigned int MAGICK_SIZE = 15;
    char buffer[MAGICK_SIZE];
    stream.read(buffer, MAGICK_SIZE);
    std::string bit_of_magick;
    for (MagickMap::const_iterator it = magick_format_map_.begin(), end = magick_format_map_.end(); it
            != end; ++it) {
        assert(it->first.size() < MAGICK_SIZE);
        if (it->first.compare(0, it->first.size(), buffer, it->first.size()) == 0)
            return it->second;
    }
    return FORMAT_UNKNOWN;
}

std::vector<std::string> ImageFormatDetector::knownExtensions() const
{
    std::vector<std::string> ret;
    ret.reserve(extension_format_map_.size());
    FormatMap::const_iterator it = extension_format_map_.begin(), end = extension_format_map_.end();
    for(; it != end; ++it)
        ret.push_back(it->first);
    return ret;
}

void ImageFormatDetector::addExt(const char * ext, image_format_t format)
{
    extension_format_map_.insert(FormatMap::value_type(ext, format));
}

void ImageFormatDetector::addMagick(const char * magick, image_format_t format)
{
    magick_format_map_.insert(MagickMap::value_type(magick, format));
}

}
