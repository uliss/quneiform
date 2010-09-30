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

#include "bmpimageexporter.h"
#include "cfcompat.h"
#include "common/imagerawdata.h"

namespace cf
{

BmpImageExporter::BmpImageExporter() {
    setFormat(FORMAT_BMP);
}

std::string BmpImageExporter::mime() const {
    return "image/x-ms-bmp";
}

void BmpImageExporter::save(const ImageRawData& image, std::ostream& os) {
    if (image.isNull())
        throw Exception("[BmpImageExporter::save] null image given");

    BITMAPFILEHEADER bf; //  bmp fileheader
    BITMAPINFOHEADER * bfinfo = (BITMAPINFOHEADER *) image.data();

    // uliss: TODO! check for endianness
    bf.bfType = 0x4d42; // 'BM'
    bf.bfSize = sizeof(BITMAPFILEHEADER) + image.dataSize();
    // fileheader + infoheader + palette
    bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bfinfo->biClrUsed
            * sizeof(RGBQUAD);

    os.write((char*) &bf, sizeof(bf));

    if (os.fail())
        throw Exception("[BmpImageExporter::save] failed");

    os << image;

    if (os.fail())
        throw Exception("[BmpImageExporter::save] failed");
}

ImageExporter::FormatList BmpImageExporter::supportedFormats() const {
    return FormatList(1, FORMAT_BMP);
}

}
