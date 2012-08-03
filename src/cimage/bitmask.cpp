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

#include <string.h> // for memset

#include "bitmask.h"
#include "cimage_debug.h"
#include "common/ctdib.h"

namespace cf
{

BitMask::BitMask(const Size& sz) :
    size_(sz),
    mask_(0)
{
    allocate();
    fill(0);
}

BitMask::BitMask(uint width, uint height) :
    size_((int) width, (int) height),
    mask_(0)
{
    allocate();
    fill(0);
}

BitMask::BitMask(uint width, uint height, uchar * data) :
    size_((int) width, (int) height),
    mask_(0)
{
    allocate();
    fill(0);
    memcpy(data, mask_, maskSize());
}

BitMask::~BitMask() {
    delete[] mask_;
}

bool BitMask::apply(CTDIB * dib) const
{
    if(!dib) {
        CIMAGE_ERROR_FUNC << "NULL image given";
        return false;
    }

    if(dib->lineWidth() != size_.width()) {
        CIMAGE_ERROR_FUNC << "image and mask widths not equal";
        return false;
    }

    if(dib->linesNumber() != size_.height()) {
        CIMAGE_ERROR_FUNC << "image and mask heights not equal";
        return false;
    }

    if(!size_.isValid()) {
        CIMAGE_ERROR_FUNC << "invalid image mask size";
        return false;
    }

    switch (dib->bpp()) {
    case 1:
        applyTo1Bit(dib);
        break;
    case 8:
        applyTo8Bit(dib);
        break;
    case 24:
        applyTo24Bit(dib);
        break;
    case 32:
        applyTo32Bit(dib);
        break;
    default:
        CIMAGE_ERROR_FUNC << "image depth is not supported:" << dib->bpp();
        return false;
    }

    return true;
}

void BitMask::fill(bool value)
{
    memset(mask_, value ? 0xFF : 0, maskSize());
}

void BitMask::fillRect(const Rect& r, bool value)
{
    for(int i = r.x(); i < r.right() && i < size_.width(); i++) {
        for(int j = r.y(); j < r.bottom() && j < size_.height(); j++) {
            if(value)
                set(i, j);
            else
                unset(i, j);
        }
    }
}

void BitMask::invert()
{
    for(uint i = 0; i < maskSize(); i++)
        mask_[i] = (~mask_[i]);
}

bool BitMask::isSet(uint x, uint y) const
{
    if(!inRange(x, y))
        return false;

    return check_(bit(x, y));
}

void BitMask::set(uint x, uint y)
{
    if(!inRange(x, y))
        return;

    uint b = bit(x, y);
    mask_[b / BITS] |= (0x80 >> (b % BITS));
}

void BitMask::unset(uint x, uint y)
{
    if(!inRange(x, y))
        return;

    uint b = bit(x, y);
    mask_[b/BITS] &= ~(0x80 >> (b % BITS));
}

void BitMask::allocate()
{
    mask_ = new uchar[maskSize()];
}

void BitMask::applyTo1Bit(CTDIB * dib) const
{
    assert(dib);

    const uchar white_pixel = (uchar) dib->whitePixel();

    for (int y = 0; y < size_.height(); y++) {
        puchar line = (puchar) dib->lineAt(y);

        for(int x = 0; x < size_.width(); x++) {
            if(!isSet(x, y)) {
                if(white_pixel == 1)
                    line[x / BITS] |= (0x80 >> (x % BITS));
                else
                    line[x / BITS] &= (0x80 >> (x % BITS));
            }
        }
    }
}

void BitMask::applyTo8Bit(CTDIB * dib) const
{
    assert(dib);

    uchar white_pixel = (uchar) dib->whitePixel();

    for (int x = 0; x < size_.width(); x++) {
        for(int y = 0; y < size_.height(); y++) {
            if(!isSet(x, y)) {
                puchar pixel = (puchar) dib->lineAt(y) + x;
                (*pixel) = white_pixel;
            }
        }
    }
}

void BitMask::applyTo24Bit(CTDIB * dib) const
{
    assert(dib);

    const int BYTES = 24 / 8;
    uchar white_pixel = (uchar) dib->whitePixel();

    for (int x = 0; x < size_.width(); x++) {
        for(int y = 0; y < size_.height(); y++) {
            if(!isSet(x, y)) {
                puchar pixel = (puchar) dib->lineAt(y) + x * BYTES;
                memset(pixel, white_pixel, BYTES);
            }
        }
    }
}

void BitMask::applyTo32Bit(CTDIB * dib) const
{
    assert(dib);

    const int BYTES = 32 / 8;
    uchar white_pixel = (uchar) dib->whitePixel();

    for (int x = 0; x < size_.width(); x++) {
        for(int y = 0; y < size_.height(); y++) {
            if(!isSet(x, y)) {
                puchar pixel = (puchar) dib->lineAt(y) + x * BYTES;
                memset(pixel, white_pixel, BYTES - 1);
            }
        }
    }
}

bool BitMask::check_(uint bit) const
{
    return (mask_[bit/BITS] & (0x80 >> (bit % BITS)));
}

bool BitMask::inRange(uint x, uint y) const
{
    return x < size_.width() && y < size_.height();
}

uint BitMask::maskSize() const
{
    return byteLineWidth() * size_.height();
}

uint BitMask::bit(uint x, uint y) const
{
    return size_.width() * y + x;
}

uint BitMask::byteLineWidth() const
{
    return (size_.width() + 7) / 8;
}

}

std::ostream& operator<<(std::ostream& os, const cf::BitMask& mask)
{
    for(int row = 0; row < mask.size().height(); row++) {
        for(int col = 0; col < mask.size().width(); col++) {
            if(mask.isSet(col, row))
                os << 1;
            else
                os << 0;
        }

        os << "\n";
    }

    os << "\n";

    return os;
}

