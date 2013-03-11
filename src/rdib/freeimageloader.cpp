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
#include "common/log.h"
#include "common/imageurl.h"
#include "common/helper.h"

namespace cf {

namespace {

cf::ImageLoader * create() {
    return new cf::FreeImageLoader;
}

void ErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) {
    cfError(MODULE_RDIB) << FreeImage_GetFormatFromFIF(fif) << message;
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
    cf::ImageLoaderFactory::instance().registerCreator(cf::FORMAT_TIFF, 300, create);
    return true;
}

bool registered = registerImageFormats();

}

FIBITMAP * convertTo24bpp(FIBITMAP * dib)
{
    FIBITMAP * new_dib = FreeImage_ConvertTo24Bits(dib);
    if(!new_dib)
        throw ImageLoader::Exception() << METHOD_SIGNATURE() << "Can't convert image to 24bpp";

    FreeImage_Unload(dib);
    return new_dib;
}

FreeImageLoader::FreeImageLoader()
{
}

ImagePtr FreeImageLoader::load(const ImageURL& url)
{
    if(!url.exists())
        throw Exception() << METHOD_SIGNATURE() << "File not exists:" << url;

    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(url.path().c_str(), 0);

    if(format == FIF_UNKNOWN)
        format = FreeImage_GetFIFFromFilename(url.path().c_str());

    if(format == FIF_UNKNOWN)
        throw Exception() << METHOD_SIGNATURE() << "Unknown image format";

    if(!FreeImage_FIFSupportsReading(format))
        throw Exception() << METHOD_SIGNATURE() << "Unsupported image format:" << FreeImage_GetFormatFromFIF(format);

    FIBITMAP * dib = NULL;
    FIMULTIBITMAP *  multi_image = NULL;

    if(url.simple()) {
        dib = FreeImage_Load(format, url.path().c_str(), 0);
    }
    else {
        multi_image = FreeImage_OpenMultiBitmap(format, url.path().c_str(), FALSE, TRUE, TRUE);
        if(!multi_image)
            throw Exception() << METHOD_SIGNATURE() << "Invalid multipage image";

        int page_count = FreeImage_GetPageCount(multi_image);
        if(url.imageNumber() < 0 || page_count <= url.imageNumber())
            throw Exception() << METHOD_SIGNATURE() << "Invalid image number";

        cfDebug(MODULE_RDIB) << METHOD_SIGNATURE() << "multi page image:" << page_count << "pages";
        cfDebug(MODULE_RDIB) << METHOD_SIGNATURE() << "loading page:" << url.imageNumber();

        dib = FreeImage_LockPage(multi_image, url.imageNumber());
        if(!dib) {
            FreeImage_CloseMultiBitmap(multi_image);
            throw Exception() << METHOD_SIGNATURE() << "Can't load image:" << url;
        }
    }

    if(!dib)
        throw Exception() << METHOD_SIGNATURE() << "Can't load image:" << url;

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

    if(multi_image) {
        FreeImage_UnlockPage(multi_image, dib, FALSE);
        // uliss: crashes
//        FreeImage_CloseMultiBitmap(multi_image);
    }
    else {
        FreeImage_Unload(dib);
    }

    ImagePtr img(new Image(data, dib_size, Image::AllocatorNew));
    img->setFileName(url.path());
    img->setSize(Size(dib_width, dib_height));
    return img;
}

ImagePtr FreeImageLoader::load(std::istream&/* stream*/)
{
    throw Exception() << METHOD_SIGNATURE() << "loading from stream is not supported yet";
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

#undef CHECK_FORMAT

    return res;
}

}
