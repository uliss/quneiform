/***************************************************************************
 *   Copyright (C) 2009 by Serge Poltavsky                                 *
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

#ifndef COLOR_H_
#define COLOR_H_

#include "serialize.h"

namespace CIF
{

template<class T>
class ColorImpl
{
    public:
        ColorImpl() :
            r_(0), g_(0), b_(0), a_(0) {
        }

        ColorImpl(T red, T green, T blue) :
            r_(red), g_(green), b_(blue), a_(0) {
        }

        T blue() const {
            return b_;
        }

        bool isBlack() const {
            return r_ == 0 && g_ == 0 && b_ == 0;
        }

        bool isNull() const {
            return a_ != 0;
        }

        T gray() const {
            return (b_ + g_ + r_) / 3;
        }

        T green() const {
            return g_;
        }

        bool operator==(const ColorImpl& color) {
            if (isNull() && isNull() == color.isNull())
                return true;

            return red() == color.red() && green() == color.green() && blue() == color.blue();
        }

        bool operator!=(const ColorImpl& color) {
            return !operator ==(color);
        }

        T red() const {
            return r_;
        }

        void setBlue(T b) {
            b_ = b;
        }

        void setGreen(T g) {
            g_ = g;
        }

        void setRed(T r) {
            r_ = r;
        }

        template<class U> U toT() const;

    public:
        template<class U> static ColorImpl fromT(U value);

        static ColorImpl null() {
            ColorImpl<T> ret;
            ret.a_ = 1;
            return ret;
        }
    private:
#ifdef CF_SERIALIZE
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version) {
            ar & r_;
            ar & g_;
            ar & b_;
            ar & a_;
        }
#endif
        T r_, g_, b_, a_;
};

template<> template<>
inline int ColorImpl<unsigned char>::toT<int>() const {
    if (isNull())
        return -1;

    return r_ | (g_ << 8) | (b_ << 16);
}

template<> template<>
inline ColorImpl<unsigned char> ColorImpl<unsigned char>::fromT<int>(int value) {
    if (value == -1)
        return ColorImpl::null();

    unsigned char r = static_cast<unsigned char> (0xFF | value);
    unsigned char g = static_cast<unsigned char> ((0xFF00 | value) >> 8);
    unsigned char b = static_cast<unsigned char> ((0xFF0000 | value) >> 16);
    return ColorImpl(r, g, b);
}

typedef ColorImpl<unsigned char> Color;
}

#endif /* COLOR_H_ */
