/***************************************************************************
 *   Copyright (C) 2012 by Serge Poltavski                                 *
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

#include <FreeImage.h>
#include <algorithm>
#include <cmath>
#include <cstring>

#include "freeimageloader.h"
#include "imageloaderfactory.h"
#include "common/debug.h"
#include "common/filesystem.h"

namespace cf {

namespace {

cf::ImageLoader * create() {
    return new cf::FreeImageLoader;
}

void ErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
    Debug() << "[RDIB] ERROR: " << FreeImage_GetFormatFromFIF(fif) << " " << message << "\n";
}

bool initFreeImage()
{
#ifdef FREEIMAGE_LIB
    FreeImage_Initialise();
#endif

    FreeImage_SetOutputMessage(ErrorHandler);
    return true;
}

bool init = initFreeImage();

bool registerImageFormats() {
    cf::ImageLoaderFactory::instance().registerCreator(cf::FORMAT_BMP, 90, create);
    cf::ImageLoaderFactory::instance().registerCreator(cf::FORMAT_GIF, 90, create);
    cf::ImageLoaderFactory::instance().registerCreator(cf::FORMAT_JPEG, 90, create);
    cf::ImageLoaderFactory::instance().registerCreator(cf::FORMAT_PNG, 90, create);
    cf::ImageLoaderFactory::instance().registerCreator(cf::FORMAT_PNM, 90, create);
    cf::ImageLoaderFactory::instance().registerCreator(cf::FORMAT_TIFF, 90, create);
    return true;
}

bool registered = registerImageFormats();

}

FIBITMAP * convertTo24bpp(FIBITMAP * dib)
{
    FIBITMAP * new_dib = FreeImage_ConvertTo24Bits(dib);
    if(!new_dib)
        throw ImageLoader::Exception("Can't convert image to 24bpp");

    FreeImage_Unload(dib);
    return new_dib;
}

FreeImageLoader::FreeImageLoader()
{
}

FreeImageLoader::~FreeImageLoader()
{
}

ImagePtr FreeImageLoader::load(const std::string& filename)
{
    if(!fs::fileExists(filename))
        throw Exception("File not exists");

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(), 0);

    if(format == FIF_UNKNOWN)
        format = FreeImage_GetFIFFromFilename(filename.c_str());

    if(format == FIF_UNKNOWN)
        throw Exception("Unknown image format");

    if(!FreeImage_FIFSupportsReading(format))
        throw Exception("Unsupported image format");

    FIBITMAP * dib = FreeImage_Load(format, filename.c_str(), 0);

    if(!dib)
        throw Exception("Can't load image");

    uint res_x = FreeImage_GetDotsPerMeterX(dib);
    uint res_y = FreeImage_GetDotsPerMeterY(dib);

    switch(FreeImage_GetBPP(dib)) {
    default:
        dib = convertTo24bpp(dib);
        break;
    }

    size_t dib_size = FreeImage_GetDIBSize(dib);
    int dib_width = (int) FreeImage_GetWidth(dib);
    int dib_height = (int) FreeImage_GetHeight(dib);
    static const double INCH = 0.0254;

    if(!res_x)
        res_x = (uint) ceil(75 / INCH + 0.5);
    if(!res_y)
        res_y = (uint) (75 / INCH);

    FreeImage_SetDotsPerMeterX(dib, res_x);
    FreeImage_SetDotsPerMeterY(dib, res_y);

    uchar * data = new uchar[dib_size];
    memcpy(data, FreeImage_GetInfo(dib), dib_size);
    FreeImage_Unload(dib);
    ImagePtr img(new Image(data, dib_size, Image::AllocatorNew));
    img->setFileName(filename);
    img->setSize(Size(dib_width, dib_height));
    return img;
}

ImagePtr FreeImageLoader::load(std::istream&/* stream*/)
{
    throw Exception("[FreeImageLoader::load] loading from stream is not supported yet");
}

ImageFormatList FreeImageLoader::supportedFormats() const
{
    ImageFormatList res;

#define CHECK_FORMAT(fif, cformat) if(FreeImage_FIFSupportsReading(fif)) {\
    if(std::find(res.begin(), res.end(), cformat) == res.end())\
       res.push_back(cformat);\
}

    CHECK_FORMAT(FIF_BMP, FORMAT_BMP);
    CHECK_FORMAT(FIF_GIF, FORMAT_GIF);
    CHECK_FORMAT(FIF_JPEG, FORMAT_JPEG);
    CHECK_FORMAT(FIF_PNG, FORMAT_PNG);
    CHECK_FORMAT(FIF_PPM, FORMAT_PNM);
    CHECK_FORMAT(FIF_PGM, FORMAT_PNM);
    CHECK_FORMAT(FIF_PBM, FORMAT_PNM);
    CHECK_FORMAT(FIF_TIFF, FORMAT_TIFF);
    CHECK_FORMAT(FIF_XPM, FORMAT_XPM);

    return res;
}

}
