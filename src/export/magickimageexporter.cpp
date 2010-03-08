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

#include <Magick++.h>
#include "magickimageexporter.h"

namespace CIF
{

MagickImageExporter::MagickImageExporter(image_format_t format) {
    setFormat(format);
}

void MagickImageExporter::save(void * data, size_t dataSize, std::ostream& os) {
    if (!data || !dataSize)
        throw Exception("[MagickImageExporter::save] bad image given");

    if (os.fail())
        throw Exception("[MagickImageExporter::save] invalid stream");

    Magick::Blob blob(data, dataSize);
    try {
        Magick::Image image;
        image.verbose(true);
        image.magick("DIB");
        image.read(blob);
        image.magick(formatToString(format()));
        image.write(&blob);
        os.write((char*) blob.data(), blob.length());
    } catch (Magick::Exception &e) {
        std::cerr << e.what() << "\n";
        throw Exception("MagickImageExporter::load failed");
    }
}

ImageExporter::FormatList MagickImageExporter::supportedFormats() const {
    // uliss: FIXME this is stupid
    FormatList formats;
    formats.push_back(FORMAT_GIF);
    formats.push_back(FORMAT_JPEG);
    formats.push_back(FORMAT_PNG);
    return formats;
}

}