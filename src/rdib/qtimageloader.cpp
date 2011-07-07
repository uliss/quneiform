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

#include <QtCore/QString>
#include <QtGui/QImage>

#include "qtimageloader.h"
#include "compat_defs.h"
#include "imageloaderfactory.h"

#ifdef WIN32
#include <windows.h>
#include <wingdi.h>
#endif

namespace
{

cf::ImageLoader * create() {
    return new cf::QtImageLoader;
}

bool png = cf::ImageLoaderFactory::instance().registerCreator(cf::FORMAT_PNG, 50, create);
bool jpeg = cf::ImageLoaderFactory::instance().registerCreator(cf::FORMAT_JPEG, 50, create);

}

namespace cf
{

static void set_dib_header(::BITMAPINFO * dibInfo, const QImage& raster) {
    memset(dibInfo, 0, sizeof(*dibInfo));
    dibInfo->bmiHeader.biSize = sizeof(::BITMAPINFOHEADER);
    dibInfo->bmiHeader.biWidth = raster.width();
    dibInfo->bmiHeader.biHeight = raster.height();
    dibInfo->bmiHeader.biPlanes = 1;
    dibInfo->bmiHeader.biBitCount = static_cast<uint16_t>(raster.depth());
    dibInfo->bmiHeader.biCompression = 0; // BI_RGB - uncompresed
    dibInfo->bmiHeader.biSizeImage = 0;
    dibInfo->bmiHeader.biXPelsPerMeter = 4 * raster.dotsPerMeterX();
    dibInfo->bmiHeader.biYPelsPerMeter = 4 * raster.dotsPerMeterY();
    // for 32-bit images return numColors() returns 0
    dibInfo->bmiHeader.biClrUsed = raster.numColors();
    dibInfo->bmiHeader.biClrImportant = 0;
}

// size of dib header (without palette)
static size_t get_dib_header_size(const ::BITMAPINFO& dib_info) {
    return dib_info.bmiHeader.biSize;
}

// size of dib palette
static size_t get_dib_palette_size(const ::BITMAPINFO& dib_info) {
    // 32-bit image
    if(dib_info.bmiHeader.biClrUsed == 0)
        return sizeof(::RGBQUAD);
    else
        return dib_info.bmiHeader.biClrUsed * sizeof(::RGBQUAD); // TODO
}

// total dib info size
static size_t get_dib_info_size(const ::BITMAPINFO& dib_info) {
    return get_dib_header_size(dib_info) + get_dib_palette_size(dib_info);
}

static uchar * make_dib(size_t size) {
    uchar * dib = new uchar[size];
    memset(dib, 0x00, size);
    return dib;
}

// copy only header
static void copy_dib_header(uchar * dib, const ::BITMAPINFO& dib_info) {
    memcpy(dib, &dib_info, get_dib_header_size(dib_info));
}

static void copy_dib_palette(uchar * dib, const ::BITMAPINFO& dib_info, const QImage& image) {
    if (dib_info.bmiHeader.biClrUsed != 0) {
        QVector<QRgb> color_table(image.colorTable());
        uchar * palette = dib + get_dib_header_size(dib_info);
        memcpy(palette, (const unsigned char*) color_table.data(), get_dib_palette_size(dib_info));
    }
    else {
        ::RGBQUAD rgbWhite = { 255, 255, 255, 0 };
        reinterpret_cast<BITMAPINFO*>(dib)->bmiColors[0] = rgbWhite;
    }
}

static void copy_dib_raster(uchar * dib, const ::BITMAPINFO& dib_info, const QImage& image) {
    uchar * pRaster = dib + get_dib_info_size(dib_info);
    memcpy(pRaster, image.bits(), image.numBytes());
}

QtImageLoader::QtImageLoader() {
}

ImagePtr QtImageLoader::load(const QString& path) {
    QImage img(path);
    ImagePtr res = load(img);
    return res;
}

ImagePtr QtImageLoader::load(const QImage& image) {
    if (image.isNull())
        throw Exception("[QtImageLoader::load] load failed.");

    // cuneiform crashes on other formats
    const QImage raster = image.convertToFormat(QImage::Format_RGB888).mirrored();
    ::BITMAPINFO dibInfo;
    set_dib_header(&dibInfo, raster);
    const size_t rasterSize = raster.numBytes();

    const int dibSize = get_dib_info_size(dibInfo) + rasterSize;
    uchar * pDib = make_dib(dibSize);

    copy_dib_header(pDib, dibInfo);
    copy_dib_palette(pDib, dibInfo, raster);
    copy_dib_raster(pDib, dibInfo, raster);

    Image * img = new Image(pDib, dibSize, Image::AllocatorNew);
    img->setSize(Size(image.width(), image.height()));

    return ImagePtr(img);
}

ImagePtr QtImageLoader::load(const std::string& path) {
    return load(QString::fromStdString(path));
}

ImagePtr QtImageLoader::load(std::istream& /*is*/) {
    throw Exception("[QtImageLoader::load] loading from stream is not supported yet");
}

}
