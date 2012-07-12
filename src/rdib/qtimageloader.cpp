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

#include <QString>
#include <QImage>
#include <QImageReader>
#include <QMap>
#include <QtPlugin>
#include <QFile>
#include <QDebug>

#include "qtimageloader.h"
#include "compat_defs.h"
#include "imageloaderfactory.h"
#include "common/dib.h"
#include "common/imageurl.h"
#include "common/debug.h"
#include "common/helper.h"
#include "common/stringbuffer.h"

#ifdef WIN32
#include <windows.h>
#include <wingdi.h>
#endif

#ifdef WITH_PDF
Q_IMPORT_PLUGIN(pdf_imageplugin)
#endif

#ifdef WITH_TIFF
Q_IMPORT_PLUGIN(multitiff_imageplugin)
#endif

namespace
{

static const int PDF_RENDER_DPI = 300;

cf::ImageLoader * create() {
    return new cf::QtImageLoader;
}

typedef QMap<QByteArray, cf::image_format_t> FormatMap;

void initFormatMap(FormatMap& m) {
    m["bmp"] = cf::FORMAT_BMP;
    m["gif"] = cf::FORMAT_GIF;
    m["jpg"] = cf::FORMAT_JPEG;
    m["jpeg"] = cf::FORMAT_JPEG;
    m["png"] = cf::FORMAT_PNG;
    m["pnm"] = cf::FORMAT_PNM;
    m["tiff"] = cf::FORMAT_TIFF;
    m["mtiff"] = cf::FORMAT_TIFF;
    m["xpm"] = cf::FORMAT_XPM;
#ifdef WITH_PDF
    m["pdf"] = cf::FORMAT_PDF;
#endif
}

void registerFormat(cf::image_format_t f) {
    cf::ImageLoaderFactory::instance().registerCreator(f, 50, create);
}

bool registerImageFormats() {
    cf::ImageFormatList formats = cf::QtImageLoader().supportedFormats();
    for(size_t i = 0; i < formats.size(); i++)
        registerFormat(formats[i]);

    return true;
}

bool registered = registerImageFormats();

}

namespace cf
{

static bool is_pdf_url(const ImageURL& url)
{
    return url.extension() == "pdf";
}

static bool is_tiff_url(const ImageURL& url)
{
    return url.extension() == "tif" || url.extension() == "tiff";
}

static inline QString urlToQt(const ImageURL& url)
{
    return QString::fromUtf8(url.path().c_str());
}

static void set_dib_header(BitmapInfoHeader * dibInfo, const QImage& raster) {
    memset(dibInfo, 0, sizeof(*dibInfo));
    dibInfo->biSize = sizeof(BitmapInfoHeader);
    dibInfo->biWidth = raster.width();
    dibInfo->biHeight = raster.height();
    dibInfo->biPlanes = 1;
    dibInfo->biBitCount = static_cast<uint16_t>(raster.depth());
    dibInfo->biCompression = 0; // BI_RGB - uncompresed
    dibInfo->biSizeImage = 0;
    dibInfo->biXPelsPerMeter = raster.dotsPerMeterX();
    dibInfo->biYPelsPerMeter = raster.dotsPerMeterY();
    // for 32-bit images return numColors() returns 0
    dibInfo->biClrUsed = (uint32_t) raster.numColors();
    dibInfo->biClrImportant = 0;
}

// size of dib header (without palette)
static size_t get_dib_header_size(const BitmapInfoHeader& dib_info) {
    return dib_info.biSize;
}

// size of dib palette
static size_t get_dib_palette_size(const BitmapInfoHeader& dib_info) {
    // 32-bit image
    if(dib_info.biClrUsed == 0)
        return sizeof(RGBQuad);
    else
        return dib_info.biClrUsed * sizeof(RGBQuad); // TODO
}

// total dib info size
static size_t get_dib_info_size(const BitmapInfoHeader& dib_info) {
    return get_dib_header_size(dib_info) + get_dib_palette_size(dib_info);
}

static uchar * make_dib(size_t size) {
    uchar * dib = new uchar[size];
    memset(dib, 0x00, size);
    return dib;
}

// copy only header
static void copy_dib_header(uchar * dib, const BitmapInfoHeader& dib_info) {
    memcpy(dib, &dib_info, get_dib_header_size(dib_info));
}

static void copy_dib_palette(uchar * dib, const BitmapInfoHeader& dib_info, const QImage& image) {
    if (dib_info.biClrUsed != 0) {
        QVector<QRgb> color_table(image.colorTable());
        uchar * palette = dib + get_dib_header_size(dib_info);
        memcpy(palette, (const unsigned char*) color_table.data(), get_dib_palette_size(dib_info));
    }
    else {
        RGBQuad rgbWhite(255, 255, 255, 0);
//        reinterpret_cast<BitmapHandle>(dib)->bmiColors[0] = rgbWhite;
    }
}

static void copy_dib_raster(uchar * dib, const BitmapInfoHeader& dib_info, const QImage& image) {
    uchar * pRaster = dib + get_dib_info_size(dib_info);
    memcpy(pRaster, image.bits(), (size_t) image.numBytes());
}

QtImageLoader::QtImageLoader() {
}

static QImage convertColorFormat(const QImage& image)
{
    QImage res;
    // cuneiform crashes on other formats
    if(image.depth() == 1 && image.format() == QImage::Format_Mono)
        res = image.mirrored();
    else
        res = image.convertToFormat(QImage::Format_RGB888).mirrored();

    res.setDotsPerMeterX(image.dotsPerMeterX());
    res.setDotsPerMeterY(image.dotsPerMeterY());

    return res;
}

ImagePtr QtImageLoader::fromQImage(const QImage& image)
{
    if (image.isNull())
        throw Exception() << METHOD_SIGNATURE() << "Null image given";

    const QImage raster = convertColorFormat(image);
    BitmapInfoHeader dibInfo;
    set_dib_header(&dibInfo, raster);
    const size_t rasterSize = (size_t) raster.numBytes();

    const size_t dibSize = get_dib_info_size(dibInfo) + rasterSize;
    uchar * pDib = make_dib(dibSize);

    copy_dib_header(pDib, dibInfo);
    copy_dib_palette(pDib, dibInfo, raster);
    copy_dib_raster(pDib, dibInfo, raster);

    Image * img = new Image(pDib, dibSize, Image::AllocatorNew);
    img->setSize(Size(image.width(), image.height()));

    return ImagePtr(img);
}

ImagePtr QtImageLoader::load(const ImageURL& url)
{
    QImage img;

    if(is_pdf_url(url))
        return loadPdf(url);
    else if(is_tiff_url(url))
        return loadTiff(url);
    else
        img.load(QString::fromUtf8(url.path().c_str()));

    return fromQImage(img);
}

ImagePtr QtImageLoader::load(std::istream& /*is*/)
{
    throw Exception() << "loading from stream is not supported yet";
}

ImageFormatList QtImageLoader::supportedFormats() const
{
    ImageFormatList res;
    FormatMap format_map;
    initFormatMap(format_map);

    QList<QByteArray> formats = QImageReader::supportedImageFormats();

    for(QList<QByteArray>::iterator it = formats.begin(); it != formats.end(); ++it) {
        QByteArray key = it->toLower();

        if(format_map.contains(key))
            res.push_back(format_map.value(key));
    }

    return res;
}

ImagePtr QtImageLoader::loadPdf(const ImageURL &url)
{
    if(!QFile::exists(urlToQt(url)))
        throw Exception() << METHOD_SIGNATURE() << "file not exists:" << url;

    QImageReader r(urlToQt(url), "PDF");

    if(!r.canRead())
        throw Exception() << METHOD_SIGNATURE() << "can't read PDF";

    r.setQuality(PDF_RENDER_DPI);

    if(url.imageNumber()) {
        if(url.imageNumber() >= r.imageCount())
            throw Exception() << METHOD_SIGNATURE() << "invalid page number:" << url.imageNumber();

        r.jumpToImage(url.imageNumber());
    }

    QImage img;
    if(!r.read(&img))
        throw Exception() << METHOD_SIGNATURE() << "can't read image:"<< url;

    return fromQImage(img);
}

ImagePtr QtImageLoader::loadTiff(const ImageURL& url)
{
    if(!QFile::exists(urlToQt(url)))
        throw Exception() << METHOD_SIGNATURE() << "file not exists:" << url;

    QImageReader r(urlToQt(url), "MTIFF");

    if(!r.canRead())
        throw Exception() << METHOD_SIGNATURE() << "can't load image:" << url;

    if(url.imageNumber()) {
        if(url.imageNumber() >= r.imageCount())
            throw Exception() << METHOD_SIGNATURE() << "invalid image number:" << url.imageNumber();

        r.jumpToImage(url.imageNumber());
    }

    QImage img;
    if(!r.read(&img))
        throw Exception() << METHOD_SIGNATURE() << "can't read image:"<< url;

    return fromQImage(img);
}

}
