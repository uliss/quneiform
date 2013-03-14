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
#include <string>

#include "globus.h"
#include "common/size.h"
#include "common/rect.h"

namespace cf
{

class CLA_EXPO BitMask
{
public:
    /**
     * Constructs empty bitmask
     */
    BitMask();

    /**
     * Construct bitmask filled with zeroes.
     * @param sz - bitmask dimensions
     */
    explicit BitMask(const Size& sz);
    explicit BitMask(const BitMask& mask);

    /**
     * Construct bitmask filled with zeroes.
     * @param width - bitmask width
     * @param height - bitmask height
     */
    BitMask(uint width, uint height);

    /**
     * Construct bitmask initiated from given bitmask data
     * @param width - bitmask width
     * @param height - bitmask height
     * @param data - pointer to raw bitmask data
     */
    BitMask(uint width, uint height, const uchar * data);

    /**
     * Construct bitmask initiated from given string
     * @param width - bitmask width
     * @param height - bitmask height
     * @param data - string that contains only 0 or 1: @example "1010100101"
     */
    BitMask(uint width, uint height, const std::string& data);
    ~BitMask();

    BitMask& operator=(const BitMask& mask);
    bool operator==(const BitMask& bm) const;
    bool operator!=(const BitMask& bm) const;

    void fill(bool value);
    void fillRect(const Rect& r, bool value);
    void invert();
    bool isSet(uint x, uint y) const;
    void set(uint x, uint y);

    /**
     * Sets mask data
     * @param data - string that contains only 0 or 1: "010100100"
     */
    void set(const std::string& data);
    void set(const BitMask& mask);
    void unset(uint x, uint y);

    Size size() const { return size_; }
    int height() const { return size_.height(); }
    int width() const { return size_.width(); }
private:
    void allocate();
    uint bit(uint x, uint y) const;
    uint byteLineWidth() const;
    bool check_(uint bit) const;
    bool inRange(uint x, uint y) const;
    // Returns mask size in bytes
    uint maskSize() const;
private:
    static const int BITS = 8;
    Size size_;
    uchar * mask_;
};

FUN_EXPO__ std::ostream& operator<<(std::ostream& os, const BitMask& mask);

}

#endif // BITMASK_H
