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

#include <QtGui>
#include <QImage>

#include "qtimageloader.h"
#include "compat_defs.h"

namespace CIF
{

QtImageLoader::QtImageLoader() {
}

QtImageLoader::~QtImageLoader() {
}

ImagePtr QtImageLoader::load(const std::string& path) {
    QImage image;

    if (!image.load(path.c_str()))
        throw Exception("[QtImageLoader::load] load failed.");

    /*.rgbSwapped();*/
    const QImage& raster = image.mirrored();
    BITMAPINFO dibInfo = { 0 };
    dibInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    dibInfo.bmiHeader.biWidth = raster.width();
    dibInfo.bmiHeader.biHeight = raster.height();
    dibInfo.bmiHeader.biPlanes = 1;
    dibInfo.bmiHeader.biBitCount = raster.depth();
    dibInfo.bmiHeader.biCompression = 0; // BI_RGB - uncompresed
    dibInfo.bmiHeader.biSizeImage = 0;
    dibInfo.bmiHeader.biXPelsPerMeter = 4 * raster.dotsPerMeterX();
    dibInfo.bmiHeader.biYPelsPerMeter = 4 * raster.dotsPerMeterY();

    QVector<QRgb> colorTable(raster.colorTable());
    /*if (32 == raster.depth())
     {
     nColors
     }
     else
     {
     }
     colorCount()
     */
    dibInfo.bmiHeader.biClrUsed = raster.numColors();
    dibInfo.bmiHeader.biClrImportant = 0;
    const size_t sizeRaster = raster.numBytes();
    unsigned int sizeInfo = dibInfo.bmiHeader.biSize;
    unsigned int sizePalete = 0;
    if (0 != dibInfo.bmiHeader.biClrUsed) {
        sizePalete = dibInfo.bmiHeader.biClrUsed * sizeof(RGBQUAD); // TODO
    } else {
        sizePalete = sizeof(RGBQUAD);
    }

    RGBQUAD rgbWhite = { 255, 255, 255, 0 };
    dibInfo.bmiColors[0] = rgbWhite;

    const int dib_size = sizeInfo + sizePalete + sizeRaster;
    uchar* pDib = new uchar[dib_size];
    memset(pDib, 0x00, dib_size);
    memcpy(pDib, &dibInfo, sizeInfo);
    if (0 != sizePalete) {
        memcpy(pDib + sizeInfo, (const unsigned char*) colorTable.data(), sizePalete);
    }

    uchar* pRaster = pDib + sizePalete + sizeInfo;
    memcpy(pRaster, raster.bits(), sizeRaster);

    Image * img = new Image(pDib, dib_size, Image::AllocatorNew);
    img->setSize(Size(image.width(), image.height()));
    return ImagePtr(img);
}

ImagePtr QtImageLoader::load(std::istream& is) {
    throw Exception("[QtImageLoader::load] loading from stream is not supported yet");
}

}
