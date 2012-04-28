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

#include <cassert>
#include <algorithm>
#include <sstream>

#include "thresholdbinarizator.h"
#include "rimage_debug.h"
#include "rdib/ctdib.h"

std::string toStr(const RGBQUAD * q) {
    std::ostringstream buf;
    buf << (int) q->rgbRed << "," << (int) q->rgbGreen << "," << (int) q->rgbBlue;
    return buf.str();
}

namespace cf {

inline static int grayAverage(const CTDIBRGBQUAD * q)
{
    return (q->rgbRed + q->rgbGreen + q->rgbBlue) / 3;
}

inline static int grayLuminance(const CTDIBRGBQUAD * q)
{
    return (q->rgbRed * 30 + q->rgbGreen * 59 + q->rgbBlue * 11) / 100;
}

inline static int grayLuma(const CTDIBRGBQUAD * q)
{
    return (q->rgbRed * 21 + q->rgbGreen * 72 + q->rgbBlue * 7) / 100;
}

inline static int grayRed(const CTDIBRGBQUAD * q)
{
    return q->rgbRed;
}

inline static int grayGreen(const CTDIBRGBQUAD * q)
{
    return q->rgbGreen;
}

inline static int grayBlue(const CTDIBRGBQUAD * q)
{
    return q->rgbBlue;
}

inline static int grayDecompositionMax(const CTDIBRGBQUAD * q)
{
    return std::max(q->rgbRed, std::max(q->rgbGreen, q->rgbBlue));
}

inline static int grayDecompositionMin(const CTDIBRGBQUAD * q)
{
    return std::min(q->rgbRed, std::min(q->rgbGreen, q->rgbBlue));
}

inline static int grayDesaturation(const CTDIBRGBQUAD * q)
{
    return (grayDecompositionMax(q) + grayDecompositionMin(q)) / 2;
}

inline static void setBlack(uchar * pixel, uint shift)
{
     (*pixel) &= ~(0x80 >> shift);
}

inline static void setWhite(uchar * pixel, uint shift)
{
    (*pixel) |= (0x80 >> shift);
}

inline static void binarizeRGBPixel(const CTDIBRGBQUAD * q, uchar * pixel, uint pixelShift,
                                    int threshold,
                                    ThresholdBinarizator::grayscale_method_t m)
{
    int gray = 0;

    switch(m) {
    case ThresholdBinarizator::AVERAGE:
        gray = grayAverage(q);
        break;
    case ThresholdBinarizator::LUMINANCE:
        gray = grayLuminance(q);
        break;
    case ThresholdBinarizator::LUMA:
        gray = grayLuma(q);
        break;
    case ThresholdBinarizator::DESATURAION:
        gray = grayDesaturation(q);
        break;
    case ThresholdBinarizator::DECOMPOSITION_MAX:
        gray = grayDecompositionMax(q);
        break;
    case ThresholdBinarizator::DECOMPOSITION_MIN:
        gray = grayDecompositionMin(q);
        break;
    case ThresholdBinarizator::CHANNEL_RED:
        gray = grayRed(q);
        break;
    case ThresholdBinarizator::CHANNEL_GREEN:
        gray = grayGreen(q);
        break;
    case ThresholdBinarizator::CHANNEL_BLUE:
        gray = grayBlue(q);
        break;
    }

    if(gray < threshold)
        setBlack(pixel, pixelShift);
    else
        setWhite(pixel, pixelShift);
}

ThresholdBinarizator::ThresholdBinarizator(int threshold) :
    threshold_(threshold),
    grayscale_method_(LUMINANCE)
{
}

CTDIB * ThresholdBinarizator::binarize(int)
{
    if(!source()) {
        RIMAGE_ERROR << "source image not set\n";
        return NULL;
    }

    CTDIB * dest = createDestination();

    if(!dest) {
        RIMAGE_ERROR << " can't create destination dib\n";
        return NULL;
    }

    const uint height = source()->GetLinesNumber();
    const uint width = (uint) source()->GetImageWidth();

    for(uint y = 0; y < height; y++) {
        for(uint x = 0; x < width; x++) {
            CTDIBRGBQUAD color;
            source()->pixelColor(x, y, &color);
            uchar * dest_pixel = (uchar*) dest->GetPtrToPixel(x, y);
            uint pixel_shift = x % 8;

            binarizeRGBPixel(&color, dest_pixel, pixel_shift, threshold_, grayscale_method_);
        }
    }

    return dest;
}

ThresholdBinarizator::grayscale_method_t ThresholdBinarizator::grayscaleMethod() const
{
    return grayscale_method_;
}

void ThresholdBinarizator::setGrayscaleMethod(ThresholdBinarizator::grayscale_method_t m)
{
    grayscale_method_ = m;
}

void ThresholdBinarizator::setThreshold(int value)
{
    threshold_ = value;
}

int ThresholdBinarizator::threshold() const
{
    return threshold_;
}

}
