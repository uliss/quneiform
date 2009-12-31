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

#include "imageformatdetector.h"
#include "common/helper.h"

namespace CIF
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
    magick_format_map_.insert(MagickMap::value_type("\0xFF\0xD8", FORMAT_JPEG));
    magick_format_map_.insert(FormatMap::value_type("\0x89PNG", FORMAT_PNG));
    magick_format_map_.insert(FormatMap::value_type("ll", FORMAT_TIFF));
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
    static const int MAGICK_SIZE = 10;
    char buffer[MAGICK_SIZE];
    stream.read(buffer, MAGICK_SIZE);
    for (MagickMap::const_iterator it = magick_format_map_.begin(), end = magick_format_map_.end(); it
            != end; ++it) {
        if(it->first.compare(0, it->first.size(), buffer) == 0)
            return it->second;
    }
    return FORMAT_UNKNOWN;
}

}
