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

namespace CIF
{

BmpImageExporter::BmpImageExporter() {
    setFormat(FORMAT_BMP);
}

void BmpImageExporter::save(void * data, size_t dataSize, std::ostream& os) {
    if (!data)
        throw Exception("[BmpImageExporter::save] invalid image data given");

    if (dataSize <= 0)
        throw Exception("[BmpImageExporter::save] invalid image size");

    BITMAPFILEHEADER bf; //  bmp fileheader
    BITMAPINFOHEADER * bfinfo = (BITMAPINFOHEADER *) data;

    // uliss: TODO! check for endianness
    bf.bfType = 0x4d42; // 'BM'
    bf.bfSize = sizeof(BITMAPFILEHEADER) + dataSize;
    // fileheader + infoheader + palette
    bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bfinfo->biClrUsed
            * sizeof(RGBQUAD);

    os.write((char*) &bf, sizeof(bf));

    if (os.fail())
        throw Exception("[BmpImageExporter::save] failed");

    os.write((char*) data, dataSize);
    if (os.fail())
        throw Exception("[BmpImageExporter::save] failed");
}

ImageExporter::FormatList BmpImageExporter::supportedFormats() const {
    return FormatList(1, FORMAT_BMP);
}

}
