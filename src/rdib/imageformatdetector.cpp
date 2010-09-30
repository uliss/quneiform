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
#include "common/helper.h"
#include "common/debug.h"

namespace cf
{

ImageFormatDetector::ImageFormatDetector()
{
    extension_format_map_.insert(FormatMap::value_type("bmp", FORMAT_BMP));
    extension_format_map_.insert(FormatMap::value_type("gif", FORMAT_GIF));
    extension_format_map_.insert(FormatMap::value_type("jpg", FORMAT_JPEG));
    extension_format_map_.insert(FormatMap::value_type("jpeg", FORMAT_JPEG));
    extension_format_map_.insert(FormatMap::value_type("png", FORMAT_PNG));
    extension_format_map_.insert(FormatMap::value_type("pnm", FORMAT_PNM));
    extension_format_map_.insert(FormatMap::value_type("pbm", FORMAT_PNM));
    extension_format_map_.insert(FormatMap::value_type("pgm", FORMAT_PNM));
    extension_format_map_.insert(FormatMap::value_type("ppm", FORMAT_PNM));
    extension_format_map_.insert(FormatMap::value_type("tiff", FORMAT_TIFF));
    extension_format_map_.insert(FormatMap::value_type("tif", FORMAT_TIFF));
    extension_format_map_.insert(FormatMap::value_type("xpm", FORMAT_XPM));

    magick_format_map_.insert(MagickMap::value_type("BM", FORMAT_BMP));
    magick_format_map_.insert(MagickMap::value_type("GIF", FORMAT_GIF));
    magick_format_map_.insert(MagickMap::value_type("\xFF\xD8", FORMAT_JPEG));
    magick_format_map_.insert(FormatMap::value_type("\x89PNG", FORMAT_PNG));
    magick_format_map_.insert(FormatMap::value_type("II", FORMAT_TIFF));
    magick_format_map_.insert(FormatMap::value_type("MM", FORMAT_TIFF));
    magick_format_map_.insert(FormatMap::value_type("P1", FORMAT_PNM));
    magick_format_map_.insert(FormatMap::value_type("P2", FORMAT_PNM));
    magick_format_map_.insert(FormatMap::value_type("P3", FORMAT_PNM));
    magick_format_map_.insert(FormatMap::value_type("P4", FORMAT_PNM));
    magick_format_map_.insert(FormatMap::value_type("P5", FORMAT_PNM));
    magick_format_map_.insert(FormatMap::value_type("P6", FORMAT_PNM));
    magick_format_map_.insert(FormatMap::value_type("/* XPM */", FORMAT_XPM));
}

ImageFormatDetector& ImageFormatDetector::instance()
{
    static ImageFormatDetector instance;
    return instance;
}

image_format_t ImageFormatDetector::detect(const std::string& filename) const
{
    std::string ext(getFileExt(filename));
    toLower(ext);
    FormatMap::const_iterator it = extension_format_map_.find(ext);
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

}
