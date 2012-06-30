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

#ifndef BITMASK_H
#define BITMASK_H

#include <iostream>

#include "globus.h"
#include "common/size.h"
#include "common/rect.h"

namespace cf
{

class CTDIB;

class CLA_EXPO BitMask
{
public:
    BitMask(const Size& sz);
    BitMask(uint width, uint height);
    BitMask(uint width, uint height, uchar * data);
    ~BitMask();

    bool apply(CTDIB * dib) const;

    void fill(bool value);
    void fillRect(const Rect& r, bool value);
    void invert();
    bool isSet(uint x, uint y) const;
    void set(uint x, uint y);
    void unset(uint x, uint y);

    Size size() const { return size_; }
private:
    BitMask(const BitMask&);
    void allocate();
    void applyTo1Bit(CTDIB * dib) const;
    void applyTo8Bit(CTDIB * dib) const;
    void applyTo24Bit(CTDIB * dib) const;
    void applyTo32Bit(CTDIB * dib) const;
    uint bit(uint x, uint y) const;
    uint byteLineWidth() const;
    bool check_(uint bit) const;
    bool inRange(uint x, uint y) const;
    uint maskSize() const;
private:
    static const int BITS = 8;
    Size size_;
    uchar * mask_;
};

}

std::ostream& operator<<(std::ostream& os, const cf::BitMask& mask);

#endif // BITMASK_H
