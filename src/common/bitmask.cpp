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
#include <string.h> // for memset
#include <algorithm>

#include "bitmask.h"
#include "common_debug.h"
#include "ctdib.h"

namespace cf
{

BitMask::BitMask(const Size& sz) :
    size_(sz),
    mask_(0)
{
    allocate();
    fill(false);
}

BitMask::BitMask(uint width, uint height) :
    size_((int) width, (int) height),
    mask_(0)
{
    allocate();
    fill(false);
}

BitMask::BitMask(uint width, uint height, const uchar * data) :
    size_((int) width, (int) height),
    mask_(0)
{
    allocate();
    fill(false);
    memcpy(mask_, data, maskSize());
}

BitMask::BitMask(uint width, uint height, const std::string& data)
    : size_((int) width, (int) height),
      mask_(0)
{
    allocate();
    fill(0);
    set(data);
}

BitMask::~BitMask() {
    delete[] mask_;
}

bool BitMask::operator==(const BitMask& bm) const
{
    if(size_ != bm.size_)
        return false;

    for(int y = 0; y < height(); y++) {
        for(int x = 0; x < width(); x++) {
            uint bit_pos = bit(x, y);
            if(check_(bit_pos) != bm.check_(bit_pos))
                return false;
        }
    }

    return true;
}

bool BitMask::operator!=(const BitMask& bm) const
{
    return !(this->operator==(bm));
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

void BitMask::set(const std::string& data)
{
    const int total = std::min((size_t) size_.width() * size_.height(), data.length());
    for(int i = 0; i < total; i++) {
        char digit = data[i];
        if(digit == '1')
            mask_[i / BITS] |= (0x80 >> (i % BITS));
        else if(digit == '0')
            mask_[i / BITS] &= ~(0x80 >> (i % BITS));
        else
            continue;
    }
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

std::ostream& operator<<(std::ostream& os, const BitMask& mask)
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

    return os;
}

}

