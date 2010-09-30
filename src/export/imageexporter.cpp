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

#include <fstream>
#include <algorithm>
#include "imageexporter.h"
#include "common/debug.h"

using namespace std;

namespace cf
{

ImageExporter::ImageExporter() :
    format_(FORMAT_UNKNOWN) {

}

ImageExporter::~ImageExporter() {
}

std::string ImageExporter::extension() const {
    switch (format_) {
    case FORMAT_BMP:
        return "bmp";
    case FORMAT_GIF:
        return "gif";
    case FORMAT_JPEG:
        return "jpg";
    case FORMAT_PNG:
        return "png";
    case FORMAT_PNM:
        return "pnm";
    case FORMAT_TIFF:
        return "tif";
    case FORMAT_XPM:
        return "xmp";
    default:
        return "";
    }
}

std::string ImageExporter::formatToString(image_format_t format) {
    switch (format) {
    case FORMAT_GIF:
        return "GIF";
    case FORMAT_JPEG:
        return "JPG";
    case FORMAT_PNG:
        return "PNG";
    case FORMAT_PNM:
        return "PNM";
    case FORMAT_TIFF:
        return "TIF";
    case FORMAT_XPM:
        return "XPM";
    default:
        return "???";
    }
}

image_format_t ImageExporter::format() const {
    return format_;
}

bool ImageExporter::isSupportedFormat(image_format_t format) {
    FormatList formats = supportedFormats();
    return std::find(formats.begin(), formats.end(), format) != formats.end();
}

void ImageExporter::setFormat(image_format_t format) {
    if (isSupportedFormat(format))
        format_ = format;
}

void ImageExporter::save(const ImageRawData& image, const string& path) {
    ofstream file;
    file.open(path.c_str(), ios::out | ios::binary | ios::trunc);
    if (!file)
        throw Exception("[ImageExporter::save] failed to: " + path);

    save(image, file);
}

}
