/*
 Copyright (c) 1993-2008, Cognitive Technologies
 All rights reserved.

 Разрешается повторное распространение и использование как в виде исходного кода,
 так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

 * При повторном распространении исходного кода должны оставаться указанное
 выше уведомление об авторском праве, этот список условий и последующий
 отказ от гарантий.
 * При повторном распространении двоичного кода в документации и/или в
 других материалах, поставляемых при распространении, должны сохраняться
 указанная выше информация об авторском праве, этот список условий и
 последующий отказ от гарантий.
 * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
 быть использованы в качестве средства поддержки и/или продвижения
 продуктов, основанных на этом ПО, без предварительного письменного
 разрешения.

 ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
 ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
 ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
 ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
 МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
 НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
 ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
 НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
 СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
 ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
 НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
 ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 * Neither the name of the Cognitive Technologies nor the names of its
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <cassert>
#include <cstdio>
#include <cmath>
#include <memory.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <bitset>

#include "ctdib.h"
#include "bmp.h"
#include "bitmask.h"
#include "common_debug.h"

using namespace cf;

namespace {

const int BITS = 8;

}

std::ostream& operator<<(std::ostream& os, const RGBQuad& c)
{
    os << "RGBQuad: "
       << static_cast<int>(c.rgbRed) << ','
       << static_cast<int>(c.rgbGreen) << ','
       << static_cast<int>(c.rgbBlue);
    return os;
}

static inline size_t dibBitsToBytes(size_t b)
{
    return ((((b + 7) / 8) + 3) / 4) * 4;
}

static inline size_t bitsToBytes(size_t a)
{
    return (a + 7) / 8;
}

static inline uint dpmToDpi(uint a)
{
    return (uint) floor((a / 100 ) * 2.54 + 0.5);
}

static inline uint dpiToDpm(uint a)
{
    return (uint) floor((a / 2.54) * 100);
}

static bool equal(const RGBQuad& q1, const RGBQuad& q2)
{
    return q1.rgbBlue == q2.rgbBlue &&
            q1.rgbGreen == q2.rgbGreen &&
            q1.rgbRed == q2.rgbRed;
}

static bool firstQUADLighterThenSecond(const RGBQuad& q1, const RGBQuad& q2)
{
    return ((q1.rgbBlue * q1.rgbBlue +
             q1.rgbGreen * q1.rgbGreen +
             q1.rgbRed * q1.rgbRed) >
            (q2.rgbBlue * q2.rgbBlue +
             q2.rgbGreen * q2.rgbGreen +
             q2.rgbRed * q2.rgbRed) ? true : false);
}

static const int CTDIB_DEFAULT_PLANES = 1;
static const int CTDIB_DEFAULT_COMPRESSION = 0;
static const int CTDIB_DEFAULT_COLORSUSED = 0;
static const int CTDIB_DEFAULT_COLORSIMPORTANT = 0;
static const int CTDIB_DEFAULT_RESOLUTION = 0;

namespace cf {

CTDIB::CTDIB() :
    valid_(false)
{
    dib_ = NULL;
    detachDIB();
    under_construction_ = false;
    direction_ = DIRECTION_UNKNOWN;
    created_by_me_ = false;
}

CTDIB::~CTDIB()
{
    destroyDIB();
}

bool CTDIB::applyMask(const BitMask& mask)
{
    if(isNull())
        return false;

    if(lineWidth() != mask.width()) {
        COMMON_ERROR_FUNC << "image and mask widths not equal";
        return false;
    }

    if(linesNumber() != mask.height()) {
        COMMON_ERROR_FUNC << "image and mask heights not equal";
        return false;
    }

    switch (bpp()) {
    case 1:
        applyTo1Bit(mask);
        break;
    case 8:
        applyTo8Bit(mask);
        break;
    case 24:
        applyTo24Bit(mask);
        break;
    case 32:
        applyTo32Bit(mask);
        break;
    default:
        COMMON_ERROR_FUNC << "image depth is not supported:" << bpp();
        return false;
    }

    return true;
}

void CTDIB::applyTo1Bit(const BitMask& mask)
{
    const uchar white_pixel = static_cast<uchar>(whitePixel());

    for (int y = 0; y < mask.height(); y++) {
        puchar line = static_cast<puchar>(lineAt(y));

        for(int x = 0; x < mask.width(); x++) {
            if(!mask.isSet(x, y)) {
                if(white_pixel == 1)
                    line[x / BITS] |= (0x80 >> (x % BITS));
                else
                    line[x / BITS] &= ~(0x80 >> (x % BITS));
            }
        }
    }
}

void CTDIB::applyTo8Bit(const BitMask& mask)
{
    const uchar white_pixel = static_cast<uchar>(whitePixel());

    for (int x = 0; x < mask.width(); x++) {
        for(int y = 0; y < mask.height(); y++) {
            if(!mask.isSet(x, y)) {
                puchar pixel = static_cast<puchar>(lineAt(y)) + x;
                (*pixel) = white_pixel;
            }
        }
    }
}

void CTDIB::applyTo24Bit(const BitMask& mask)
{
    const int BYTES = 24 / BITS;
    const uchar white_pixel = static_cast<uchar>(whitePixel());

    for (int x = 0; x < mask.width(); x++) {
        for(int y = 0; y < mask.height(); y++) {
            if(!mask.isSet(x, y)) {
                puchar pixel = static_cast<puchar>(lineAt(y)) + x * BYTES;
                memset(pixel, white_pixel, BYTES);
            }
        }
    }
}

void CTDIB::applyTo32Bit(const BitMask& mask)
{
    const int BYTES = 32 / BITS;
    const uchar white_pixel = static_cast<uchar>(whitePixel());

    for (int x = 0; x < mask.width(); x++) {
        for(int y = 0; y < mask.height(); y++) {
            if(!mask.isSet(x, y)) {
                puchar pixel = static_cast<puchar>(lineAt(y)) + x * BYTES;
                memset(pixel, white_pixel, BYTES - 1);
            }
        }
    }
}

void CTDIB::mapToPixels32(ConstFunction32 func) const
{
    if(isNull())
        return;

    mapToPixels32(func, pBitFild, width(), linesNumber());
}

void CTDIB::mapToPixels32(Function32 func)
{
    if(isNull())
        return;

    mapToPixels32(func, pBitFild, width(), linesNumber());
}

void CTDIB::mapToPixels32(CTDIB::Function32 func, void * data, size_t width, size_t height)
{
    if(!data)
        return;

    uchar * begin =  (uchar*) data;
    uchar * end = begin + (width * height * 4); // 32/8
    uint pos = 0;
    for(uchar * it = begin; it != end; it += 4) {
        func((RGBQuad*) it, pos % width, pos / height);
        ++pos;
    }
}

void CTDIB::mapToPixels32(ConstFunction32 func, const void *data, size_t width, size_t height)
{
    if(!data)
        return;

    const uchar * begin =  (uchar*) data;
    const uchar * end = begin + (width * height * 4); // 32/8
    uint pos = 0;
    for(const uchar * it = begin; it != end; it += 4) {
        func((const RGBQuad*) it, pos % width, pos / height);
        pos++;
    }
}

void CTDIB::mapToPixels24(CTDIB::ConstFunction24 func) const
{
    if(isNull())
        return;

    mapToPixels24(func, pBitFild, width(), linesNumber());
}

void CTDIB::mapToPixels24(CTDIB::Function24 func)
{
    if(isNull())
        return;

    mapToPixels24(func, pBitFild, width(), linesNumber());
}

void CTDIB::mapToPixels24(Function24 func, void * data, size_t width, size_t height)
{
    if(!data)
        return;

    uchar * line = (uchar*) data;
    const size_t line_stride = stride(width, 24);

    for(size_t y = 0; y < height; y++, line += line_stride) {
        for(size_t x = 0; x < width; x++)
            func(line + x*3, x, y); // 24/8
    }
}

void CTDIB::mapToPixels24(ConstFunction24 func, const void * data, size_t width, size_t height)
{
    if(!data)
        return;

    const uchar * line = (const uchar*) data;
    const size_t line_stride = stride(width, 24);

    for(size_t y = 0; y < height; y++, line += line_stride) {
        for(size_t x = 0; x < width; x++)
            func(line + x*3, x, y); // 24/8
    }
}

bool CTDIB::pixelColor(uint x, uint y, RGBQuad * dest) const
{
    if(!isValid())
        return false;

    void * pixel_data = pixelAt(x, y);

    if(!pixel_data)
        return false;

    if(!dest) {
        COMMON_WARNING_FUNC << "NULL destination given";
        return false;
    }

    switch(bpp()) {
    case 1:
    case 4:
    case 8:
    {
        uint color_idx = 0;
        if(!pixelColorIndex(x, y, &color_idx))
            return false;

        if(!palleteColor(color_idx, dest))
            return false;

        return true;
    }
    case 16: {
        enum {
            BI_RGB = 0,
            BI_BITFIELDS = 3
        };

        switch(info_header_->biCompression) {
        case BI_RGB: {
            // no compression
            // 0RRRRRGGGGGBBBBB - value
            uint32_t pixel = *(uint32_t*) pixel_data;
            dest->rgbRed = static_cast<uchar>((pixel & 0x7C00) >> 5);
            dest->rgbGreen = static_cast<uchar>((pixel & 0x3E0) >> 2);
            dest->rgbBlue = static_cast<uchar>((pixel & 0x1f) << 3);
            dest->rgbReserved = 0;
            return true;
        }
        case BI_BITFIELDS: {
            uint32_t pixel = *(uint32_t*) pixel_data;

            switch(version_) {
            case CTDIB::VERSION_4:
            case CTDIB::VERSION_5: {
                BitmapInfoV4Header * v4header = (BitmapInfoV4Header*) info_header_;
                dest->rgbRed = static_cast<uchar>((pixel & v4header->bV4RedMask) >> 5);
                dest->rgbGreen = static_cast<uchar>((pixel & v4header->bV4GreenMask) >> 2);
                dest->rgbBlue = static_cast<uchar>((pixel & v4header->bV4BlueMask) << 3);
                return false;
            }
            case CTDIB::VERSION_WINDOWS:
                return false;
            default:
               return false;
            }
        }
        default:
            return false;
        }
    }
    case 24: {
        uchar * rgb = (uchar*) pixel_data;
        dest->rgbBlue = rgb[0];
        dest->rgbGreen = rgb[1];
        dest->rgbRed = rgb[2];
        return true;
    }
    case 32:
        *dest = *((RGBQuad*) pixel_data);
        return true;
    default:
        return false;
    }
}

bool CTDIB::pixelColorIndex(uint x, uint y, uint * dest) const
{
    if(isNull())
        return false;

    if(!isIndexed())
        return false;

    if(!dest)
        return false;

    void * pixel_data = pixelAt(x, y);

    if(!pixel_data)
        return false;

    switch(bpp()) {
    case 1: {
        uchar pixel_byte = *((uchar*) pixel_data);
        uint pixel_shift = x % 8;
        if(pixel_byte & (0x80 >> pixel_shift))
            *dest = 1;
        else
            *dest = 0;
        return true;
    }
    case 4: {
        uchar color_pair = *((uchar*) pixel_data);
        uchar color_idx = (x % 2)
                ? (color_pair & 0xf)
                : (color_pair >> 4);

        *dest = (uint) color_idx;
        return true;
    }
    case 8: {
        uchar color_idx = *((uchar*) pixel_data);
        *dest = (uint) color_idx;
        return true;
    }
    default:
        return false;
    }
}

bool CTDIB::setPixelColor(uint x, uint y, const RGBQuad &c)
{
    if(isNull())
        return false;

    switch(bpp()) {
    case 1:
    case 4:
    case 8: {
        uint color_idx = 0;
        if(!pixelColorIndex(x, y, &color_idx))
            return false;

        int pallete_color_idx = palleteColorIndex(c);

        if(pallete_color_idx == -1) {
            int new_color_idx = addPalleteColor(c);
            if(new_color_idx == -1)
                return false;

            setPixelColorIndex(x, y, (uint) new_color_idx);
            return true;
        }

        setPixelColorIndex(x, y, (uint) pallete_color_idx);
        return true;
    }
    case 24: {
        uchar * rgb = (uchar*) pixelAt(x, y);
        if(!rgb)
            return false;

        rgb[0] = c.rgbBlue;
        rgb[1] = c.rgbGreen;
        rgb[2] = c.rgbRed;
        return true;
    }
    case 32: {
        RGBQuad * p = (RGBQuad*) pixelAt(x, y);
        if(!p)
            return false;

        *p = c;
        return true;
    }
    default:
        return false;
    }

    return false;
}

bool CTDIB::setPixelColorIndex(uint x, uint y, uint colorIdx)
{
    if(isNull())
        return false;

    if(!isIndexed())
        return false;

    uchar * pixel_data = (uchar*) pixelAt(x, y);

    if(!pixel_data)
        return false;

    switch(bpp()) {
    case 1: {
        uint pixel_shift = x % 8;

        if(colorIdx == 1) {
            *pixel_data |= (0x80 >> pixel_shift);
        }
        else if(colorIdx == 0) {
            *pixel_data &= ~(0x80 >> pixel_shift);
        }
        else
            return false;

        return true;
    }
    case 4: {
        uchar p1 = 0x0f & *pixel_data;
        uchar p2 = 0xf0 & *pixel_data;

        if(x % 2) {
            p1 = 0x0f & colorIdx;
        }
        else
            p2 = 0xf0 & (colorIdx << 4);

        *pixel_data = p1 | p2;
        return true;
    }
    case 8:
        *pixel_data = (uchar) colorIdx;
        return true;
    default:
        return false;
    }
}

bool CTDIB::fill(const RGBQuad& c)
{
    if(isNull())
        return false;

    if(isIndexed() && !palleteHasColor(c)) {
        int idx = addPalleteColor(c);
        if(idx == -1)
            return false;
    }

    for(uint y = 0; y < linesNumber(); y++) {
        for(uint x = 0; x < lineWidth(); x++) {
            if(!setPixelColor(x, y, c))
                return false;
        }
    }

    return true;
}

uint CTDIB::palleteUsedColorsNum() const
{
    if(isNull())
        return 0;

    return usedColors(info_header_->biBitCount, info_header_->biClrUsed);
}

bool CTDIB::attachDIB()
{
    if (dib_ == NULL) {
        detachDIB();
        return false;
    }

    valid_ = true;
    BitmapInfoHeader * pSimpleHead = (BitmapInfoHeader*) dib_;

    switch (pSimpleHead->biSize) {
    case DIB_VERSION_3_HEADER_SIZE:
        version_ = VERSION_WINDOWS;
        break;
    case DIB_VERSION_4_HEADER_SIZE:
        version_ = VERSION_4;
        break;
    case DIB_VERSION_5_HEADER_SIZE:
        version_ = VERSION_5;
        break;
    default:
        COMMON_ERROR_FUNC << "Unknown DIB header size: " << pSimpleHead->biSize;
        detachDIB();
        return false;
    }

    info_header_ = (BitmapInfoHeader*) dib_;
    rgb_quads_ = (RGBQuad*) ((puchar) dib_ + pSimpleHead->biSize);
    pBitFild = (puchar) (rgb_quads_ + palleteUsedColorsNum());
    valid_ = true;
    direction_ = (pSimpleHead->biHeight > 0) ? DIRECTION_BOTTOM_UP : DIRECTION_TOP_DOWN;
    return true;
}

void CTDIB::detachDIB()
{
    info_header_ = NULL;
    rgb_quads_ = NULL;
    pBitFild = NULL;
    dib_ = NULL;
    valid_ = false;
    created_by_me_ = false;
    under_construction_ = false;
    version_ = VERSION_UNKNOWN;
    direction_ = DIRECTION_UNKNOWN;
}

BitmapInfoHeader * CTDIB::header() const
{
    if(isNull())
        return NULL;

    return info_header_;
}

void * CTDIB::pallete() const
{
    if(isNull())
        return NULL;

    return (pvoid) rgb_quads_;
}

bool CTDIB::setBitmap(void * ptr)
{
    if (dib_ || !ptr)
        return false;

    dib_ = ptr;
    return attachDIB();
}

bool CTDIB::isValid() const
{
    return valid_;
}

bool CTDIB::isIndexed() const
{
    switch(bpp()) {
    case 1:
    case 4:
    case 8:
        return true;
    default:
        return false;
    }
}

int CTDIB::height() const
{
    if(isNull())
        return 0;

    return info_header_->biHeight;
}

size_t CTDIB::linesNumber() const
{
    if(isNull())
        return 0;

    return static_cast<size_t>(abs(height()));
}

int CTDIB::width() const
{
    if(isNull())
        return 0;

    return info_header_->biWidth;
}

size_t CTDIB::lineWidth() const
{
    if(isNull())
        return 0;

    return static_cast<size_t>(abs(width()));
}

size_t CTDIB::lineWidthInBytes() const
{
    if(isNull())
        return 0;

    return dibBitsToBytes(lineWidth() * bpp());
}

size_t CTDIB::lineRealWidthInBytes() const
{
    if(isNull())
        return 0;

    return bitsToBytes(lineWidth() * bpp());
}

size_t CTDIB::imageSizeInBytes() const
{
    if(isNull())
        return 0;

    return lineWidthInBytes() * linesNumber();
}

size_t CTDIB::bpp() const
{
    if(isNull())
        return 0;

    return info_header_->biBitCount;
}

static BitmapInfoHeader * createHeaderV3(int width, int height, uint bpp)
{
    BitmapInfoHeader * res = new BitmapInfoHeader;
    memset(res, 0, sizeof(BitmapInfoHeader));

    res->biSize = DIB_VERSION_3_HEADER_SIZE;
    res->biWidth = width;
    res->biHeight = height;
    res->biPlanes = CTDIB_DEFAULT_PLANES;
    res->biBitCount = (uint16_t) bpp;
    res->biCompression = CTDIB_DEFAULT_COMPRESSION;
    res->biSizeImage = (uint32_t) (dibBitsToBytes(width * bpp) * abs(height));
    res->biXPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
    res->biYPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
    res->biClrUsed = 0;
    res->biClrImportant = CTDIB_DEFAULT_COLORSIMPORTANT;

    return res;
}

static BitmapInfoV4Header * createHeaderV4(int width, int height, uint bpp)
{
    BitmapInfoV4Header * res = new BitmapInfoV4Header;
    memset(res, 0, sizeof(BitmapInfoV4Header));

    res->bV4Size = DIB_VERSION_4_HEADER_SIZE;
    res->bV4Height = height;
    res->bV4Width = width;
    res->bV4Planes = CTDIB_DEFAULT_PLANES;
    res->bV4BitCount = (uint16_t) bpp;
    res->bV4V4Compression = CTDIB_DEFAULT_COMPRESSION;
    res->bV4SizeImage = (uint32_t) dibBitsToBytes(width * bpp) * abs(height);
    res->bV4XPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
    res->bV4YPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
    res->bV4ClrUsed = 0;
    res->bV4ClrImportant = CTDIB_DEFAULT_COLORSIMPORTANT;
    return res;
}

static BitmapInfoV5Header * createHeaderV5(int width, int height, uint bpp)
{
    BitmapInfoV5Header * res = new BitmapInfoV5Header;
    memset(res, 0, sizeof(BitmapInfoV5Header));

    res->bV5Size = DIB_VERSION_5_HEADER_SIZE;
    res->bV5Height = height;
    res->bV5Width = width;
    res->bV5Planes = CTDIB_DEFAULT_PLANES;
    res->bV5BitCount = (uint16_t) bpp;
    res->bV5Compression = CTDIB_DEFAULT_COMPRESSION;
    res->bV5SizeImage = dibBitsToBytes(width * bpp) * abs(height);
    res->bV5XPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
    res->bV5YPelsPerMeter = CTDIB_DEFAULT_RESOLUTION;
    res->bV5ClrUsed = 0;
    res->bV5ClrImportant = CTDIB_DEFAULT_COLORSIMPORTANT;

    return res;
}

BitmapPtr CTDIB::createBegin(int width, int height, uint bitCount, version_t version)
{
    if (isValid())
        return NULL;

    BitmapInfoHeader * v3_header = NULL;
    BitmapInfoV4Header * v4_header = NULL;
    BitmapInfoV5Header * v5_header = NULL;

    pvoid header = NULL;
    uint header_size = 0;

    switch (version) {
    case VERSION_WINDOWS:
        header_size = DIB_VERSION_3_HEADER_SIZE;
        v3_header = createHeaderV3(width, height, bitCount);
        header = (pvoid) v3_header;
        break;
    case VERSION_4:
        header_size = DIB_VERSION_4_HEADER_SIZE;
        v4_header = createHeaderV4(width, height, bitCount);
        header = (pvoid) v4_header;
        break;
    case VERSION_5:
        header_size = DIB_VERSION_5_HEADER_SIZE;
        v5_header = createHeaderV5(width, height, bitCount);
        header = (pvoid) v5_header;
        break;
    default:
        return NULL;
    }

    uint fuel_size = header_size;
    fuel_size += usedColors(bitCount, 0) * sizeof(RGBQuad);
    fuel_size += dibBitsToBytes(width * bitCount) * abs(height);
    free(dib_);
    dib_ = calloc(1, fuel_size);
    created_by_me_ = true;
    memcpy(dib_, header, header_size);
    info_header_ = (BitmapInfoHeader*) dib_;
    delete v3_header;
    delete v4_header;
    delete v5_header;
    valid_ = true;
    under_construction_ = true;
    attachDIB();
    return (BitmapPtr) dib_;
}

uint CTDIB::usedColors(uint bitCount, uint colorUsed)
{
    if(colorUsed != 0)
        return colorUsed;

    switch (bitCount) {
    case 1:
        return 2;
    case 4:
        return 16;
    case 8:
        return 256;
    case 16:
    case 24:
    case 32:
    default:
        return 0;
    }
}

bool CTDIB::setResolutionDotsPerInch(uint x, uint y)
{
    if (!under_construction_)
        return false;

    return setResolutionDotsPerMeter(dpiToDpm(x), dpiToDpm(y));
}

bool CTDIB::setResolutionDotsPerMeter(uint x, uint y)
{
    if (!under_construction_)
        return false;

    info_header_->biXPelsPerMeter = (int32_t) x;

    if(y == 0)
        info_header_->biYPelsPerMeter = info_header_->biXPelsPerMeter;
    else
        info_header_->biYPelsPerMeter = (int32_t) y;

    return true;
}

bool CTDIB::resolutionDotsPerInch(uint * x, uint * y) const
{
    if(isNull())
        return false;

    uint x_dpi = 0;
    uint y_dpi = 0;

    if (x == NULL || !resolutionDotsPerMeter(&x_dpi, &y_dpi))
        return false;

    *x = dpmToDpi(x_dpi);

    if (y != NULL)
        *y = dpmToDpi(y_dpi);

    return true;
}

bool CTDIB::resolutionDotsPerMeter(uint * x, uint * y) const
{
    if(isNull())
        return false;

    if(x == NULL)
        return FALSE;

    *x = (uint) info_header_->biXPelsPerMeter;

    if(y != NULL)
        *y = (uint) info_header_->biYPelsPerMeter;

    return true;
}

bool CTDIB::createEnd()
{
    under_construction_ = false;
    return true;
}

void * CTDIB::lineAt(uint y) const
{
    if(isNull())
        return NULL;

    if (y >= linesNumber())
        return NULL;

    puchar line = pBitFild;

    if (direction_ == DIRECTION_BOTTOM_UP) {
        line += ((linesNumber() - (y + 1)) * lineWidthInBytes());
        return (pvoid) line;
    }

    if (direction_ == DIRECTION_TOP_DOWN) {
        line += (y * lineWidthInBytes());
        return (pvoid) line;
    }

    return NULL;
}

void * CTDIB::pixelAt(uint x, uint y) const
{
    if(isNull())
        return NULL;

    puchar pLine = NULL;

    if (x >= lineWidth())
        return NULL;

    pLine = (puchar) lineAt(y);

    if (!pLine)
        return NULL;

    pLine += ((x * bpp()) / 8);
    return pLine;
}

bool CTDIB::reset()
{
    if(isNull())
        return false;

    return destroyDIB();
}

bool CTDIB::isNull() const
{
    return !valid_;
}

bool CTDIB::bitmap(BitmapPtr * dib) const
{
    if(isNull())
        return false;

    *dib = (BitmapPtr) dib_;
    return true;
}

bool CTDIB::setPalleteColor(uint32_t idx, const RGBQuad& color)
{
    if (rgb_quads_ == NULL)
        return false;

    if (idx >= palleteUsedColorsNum())
        return false;

    RGBQuad * pCurrentQuad = (RGBQuad*) pallete();
    pCurrentQuad += idx;
    *pCurrentQuad = color;
    return true;
}

bool CTDIB::destroyDIB()
{
    if(isNull())
        return false;

    if (created_by_me_)
        free(dib_);

    detachDIB();
    return true;
}

size_t CTDIB::dibSize() const
{
    if(isNull())
        return 0;

    size_t res = 0;
    res += headerSize();
    res += palleteSize();
    res += imageSizeInBytes();
    return res;
}

size_t CTDIB::headerSize() const
{
    if(isNull())
        return 0;

    return header()->biSize;
}

size_t CTDIB::palleteSize() const
{
    if(isNull())
        return 0;

    return (palleteUsedColorsNum() * sizeof(RGBQuad));
}

bool CTDIB::palleteHasColor(const RGBQuad& c) const
{
    return palleteColorIndex(c) != -1;
}

int CTDIB::palleteColorIndex(const RGBQuad &c) const
{
    if(isNull())
        return -1;

    if(!isIndexed())
        return -1;

    for(uint i = 0; i < palleteUsedColorsNum(); i++) {
        RGBQuad tmp;
        if(!palleteColor(i, &tmp))
            return -1;

        if(tmp == c)
            return (int) i;
    }

    return -1;
}

int CTDIB::addPalleteColor(const RGBQuad& c)
{
    if(isNull())
        return -1;

    if(!isIndexed())
        return -1;

    const uint total = palleteUsedColorsNum();
    const size_t MAX_PALLETE_SIZE = 256;
    std::bitset<MAX_PALLETE_SIZE> colors_hist;

    for(uint y = 0; y < linesNumber(); y++) {
        for(uint x = 0; x < lineWidth(); x++) {
            uint color_idx = 0;
            if(!pixelColorIndex(x, y, &color_idx))
                return -1;

            if(color_idx >= total) {
                COMMON_ERROR_FUNC << "invalid color index:" << color_idx;
                return -1;
            }

            colors_hist[color_idx] = 1;
        }
    }

    if(colors_hist.count() >= total) {
        COMMON_WARNING_FUNC << "can't add color: pallete is full";
        return -1;
    }

    for(uint i = 0; i < total; i++) {
        if(!colors_hist.test(i)) {
            // unused color found
            COMMON_DEBUG_FUNC << "color set: " << i;
            setPalleteColor(i, c);
            return (int) i;
        }
    }

    return -1;
}

uint CTDIB::pixelShiftInByte(uint xPos) const
{
    if(isNull())
        return 0;

    switch (bpp()) {
    case 1:
        return xPos % 8;
    case 4:
        if (xPos % 2 == 0)
            return 0;
        else
            return 4;
    default:
        return 0;
    }
}

bool CTDIB::saveToBMP(const std::string& fileName) const
{
    if(isNull())
        return false;

    std::ofstream f(fileName.c_str(), std::ios::out | std::ios::binary);

    if(!f)
        return false;

    bool rc = saveToBMP(f);
    f.close();
    return rc;
}

bool CTDIB::saveToBMP(std::ostream& os) const
{
    if (isNull())
        return false;

    BitmapFileHeader bf; //  bmp fileheader
    // uliss: TODO! check for endianness
    bf.bfSize = static_cast<uint32_t>(sizeof(BitmapFileHeader) + dibSize());
    // fileheader + infoheader + palette
    bf.bfOffBits = static_cast<uint32_t>(sizeof(BitmapFileHeader) + headerSize() + palleteSize());

    os.write((char*) &bf, sizeof(bf));

    if (os.fail())
        return false;

    os.write((char*) header(), (std::streamsize) dibSize());

    if (os.fail())
        return false;

    os.flush();

    return true;
}

size_t CTDIB::pixelCount() const
{
    if(isNull())
        return 0;

    return width() * linesNumber();
}

bool CTDIB::saveToBMP(const std::string& fileName, BitmapPtr bitmap)
{
    CTDIB dib;
    if(!dib.setBitmap(bitmap))
        return false;

    return dib.saveToBMP(fileName);
}

bool CTDIB::saveToBMP(std::ostream& os, BitmapPtr bitmap)
{
    CTDIB dib;
    if(!dib.setBitmap(bitmap))
        return false;

    return dib.saveToBMP(os);
}

bool CTDIB::palleteColor(size_t idx, RGBQuad * dest) const
{
    if (rgb_quads_ == NULL)
        return false;

    if (idx >= palleteUsedColorsNum()) {
        COMMON_ERROR_FUNC << "invalid pallete index:" << idx;
        return false;
    }

    RGBQuad * current = (RGBQuad*) pallete();
    current += idx;
    if(dest)
        *dest = *current;

    return true;
}

void * CTDIB::imageData()
{
    if(isNull())
        return NULL;

    return pBitFild;
}

const void * CTDIB::imageData() const
{
    if(isNull())
        return NULL;

    return pBitFild;
}

CTDIB::version_t CTDIB::version() const
{
    if(isNull())
        return VERSION_UNKNOWN;

    return version_;
}

bool CTDIB::copyLineFromDIB(const CTDIB * src, uint srcLine, uint destLine, uint srcX)
{
    if(isNull())
        return false;

    if (src == NULL)
        return false;

    puchar srcStart = (puchar) src->pixelAt(srcX, srcLine);
    puchar destStart = (puchar) this->lineAt(destLine);

    if (srcStart == NULL || destStart == NULL)
        return false;

    if ((src->lineWidth() - srcX) < this->lineWidth() )
        return false;

    if (src->bpp() != bpp())
        return false;

    switch (this->bpp()) {
    case 1:
    case 4: {
        uint t = (src->lineRealWidthInBytes() > this->lineRealWidthInBytes() + ((srcX
                * bpp()) / 8)) ? 1 : 0;

        const size_t src_width = src->lineRealWidthInBytes();
        uchar * buf = new uchar[src_width];
        memset(buf, 0, src_width);
        memcpy(buf, srcStart, this->lineRealWidthInBytes() + t);

        uint pixelShift = src->pixelShiftInByte(srcX);
        if (pixelShift) {
            size_t wByte = this->lineRealWidthInBytes() + t;
            puchar pwByte = buf;
            uchar wShiftic = 0;

            while (wByte - t > 0) {
                wByte--;

                if (wByte)
                    wShiftic = *(pwByte + 1);

                *pwByte = (uchar) (*pwByte << pixelShift);

                if (wByte) {
                    wShiftic = wShiftic >> (8 - pixelShift);
                    *pwByte |= wShiftic;
                    pwByte++;
                }
            }
        }

        memcpy(destStart, buf, this->lineRealWidthInBytes());
        delete[] buf;
        break;
    }
    case 8:
    case 16:
    case 24:
    case 32:
        memcpy(destStart, srcStart, this->lineRealWidthInBytes());
        break;
    default:
        return false;
    }

    return true;
}

bool CTDIB::copyPalleteFromDIB(const CTDIB * src)
{
    if(isNull())
        return false;

    if(!src) {
        COMMON_ERROR_FUNC << "NULL source pointer";
        return false;
    }

    uint dest_colors = palleteUsedColorsNum();
    uint src_colors = src->palleteUsedColorsNum();

    if (src_colors > dest_colors) {
        COMMON_ERROR_FUNC << "source pallete bigger then destination:"
                          << src->palleteUsedColorsNum() << '>' << dest_colors;
        return false;
    }

    for (uint i = 0; i < src_colors; i++) {
        RGBQuad Quad;
        src->palleteColor(i, &Quad);

        if (!setPalleteColor(i, Quad))
            return false;
    }

    return true;
}

bool CTDIB::makeBlackAndWhitePallete()
{
    if(isNull())
        return false;

    if(!isIndexed())
        return false;

    return setPalleteColor(0, RGBQuad::black()) &&
        setPalleteColor(1, RGBQuad::white());
}

bool CTDIB::copyDPIFromDIB(const CTDIB * src)
{
    if (!under_construction_)
        return false;

    if (!src) {
        COMMON_ERROR_FUNC << "NULL source pointer";
        return false;
    }

    uint x = 0;
    uint y = 0;

    if (src->resolutionDotsPerMeter(&x, &y) && this->setResolutionDotsPerMeter(x, y))
        return true;

    return false;
}

uint32_t CTDIB::whitePixel() const
{
    if(isNull())
        return 0;

    RGBQuad fQ;
    RGBQuad sQ;
    uint32_t Color = 0;

    switch (bpp()) {
    case 1:
        if (palleteColor(0, &fQ) && palleteColor(1, &sQ)) {
            if (equal(fQ, sQ))
                return 1;

            return firstQUADLighterThenSecond(sQ, fQ);
        }
        return 0;
    case 4:
    case 8:
        if (palleteColor(0, &fQ)) {
            for (uint i = 1; i < palleteUsedColorsNum(); i++) {
                if (palleteColor(i, &sQ)) {
                    if (firstQUADLighterThenSecond(sQ, fQ)) {
                        Color = i;
                        palleteColor(i, &fQ);
                    }
                }
            }
        }
        return Color;
    case 16:
        return 0x00007fff;
    case 24:
        return 0x00ffffff;
    case 32:
        return 0xffffffff;
    default:
        return 0;
    }
}

uint32_t CTDIB::blackPixel() const
{
    if(isNull())
        return 0;

    RGBQuad fQ;
    RGBQuad sQ;
    uint32_t Color = 0;

    switch (bpp()) {
    case 1:
        if (palleteColor(0, &fQ) && palleteColor(1, &sQ))
            return firstQUADLighterThenSecond(fQ, sQ);
        return 0;
    case 4:
    case 8:
        if (palleteColor(0, &fQ)) {
            for (uint i = 1; i < palleteUsedColorsNum(); i++) {
                if (palleteColor(i, &sQ)) {
                    if (firstQUADLighterThenSecond(fQ, sQ)) {
                        Color = i;
                        palleteColor(i, &fQ);
                    }
                }
            }
        }
        return Color;
    case 16:
    case 24:
    case 32:
        return 0;
    default:
        return 0;
    }
}

}
