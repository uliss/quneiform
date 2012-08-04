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

#include "histogramcreator.h"
#include "rimage_debug.h"

namespace cf
{

bool HistogramCreator::grayBrighness(Histogram& hist, const CTDIB& dib)
{
    if(dib.isNull()) {
        RIMAGE_WARNING_FUNC() << "null DIB";
        return false;
    }

    if(dib.bpp() == 1) {
        RIMAGE_WARNING_FUNC() << "1bpp image given";
        return false;
    }

    if(hist.size() < 256) {
        RIMAGE_WARNING_FUNC() << "histogram width is to small:" << hist.size() << "resizing";
        hist.resize(256);
    }

    if(dib.bpp() == 24)
        return grayBrighnessFromRGB(hist, dib.imageData(), dib.pixelCount());

    uint h = dib.linesNumber();
    uint w = dib.width();
    RGBQuad color;
    for(uint y = 0; y < h; y++) {
        for(uint x = 0; x < w; x++) {
            if(!dib.pixelColor(x, y, &color)) {
                RIMAGE_ERROR_FUNC() << "invalid pixel:" << x << 'x' << y;
                return false;
            }

            uint gray = color.grayAverage();
            hist.add(gray);
        }
    }

    return true;
}

bool HistogramCreator::save(const Histogram& hist, const std::string& fileName)
{
    Histogram tmp = hist;
    tmp.minimize();
    const uint w = tmp.size();
    const uint h = 256;

    CTDIB image;
    if(!image.createBegin(w, h, 1)) {
        RIMAGE_ERROR_FUNC() << "can't create dib with width:" << w << "and height:" << h;
        return false;
    }
    image.makeBlackAndWhitePallete();
    if(!image.createEnd())
        return false;

    for(uint y = 0; y < h; y++) {
        for(uint x = 0; x < w; x++) {
            uint colorIdx = (tmp[x] > y) ? 0 : 1;
            image.setPixelColorIndex(x, h - y, colorIdx);
        }
    }

    return image.saveToBMP(fileName);
}

bool HistogramCreator::grayBrighnessFromRGB(Histogram& hist, const void * data, size_t pixelNum)
{
    const RGBQuad * begin = reinterpret_cast<const RGBQuad*>(data);
    const RGBQuad * end = begin + pixelNum;
    for(const RGBQuad * c = begin; c != end; c++)
        hist.add(c->grayAverage());

    return true;
}

}
