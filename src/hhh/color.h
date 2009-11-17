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

namespace CIF {

template<class T>
class ColorImpl
{
public:
    ColorImpl(T red, T green, T blue) :
        r_(red), g_(green), b_(blue), a_(0) {
    }

    ColorImpl(T red, T green, T blue, T alpha) :
        r_(red), g_(green), b_(blue), a_(alpha) {
    }

    ColorImpl() :
        r_(0), g_(0), b_(0), a_(0) {
    }

    explicit ColorImpl(T gray) :
        r_(gray), g_(gray), b_(gray), a_(0) {
    }

    T alpha() const {
        return a_;
    }

    T blue() const {
        return b_;
    }

    int get() const;

    T gray() const {
        return (b_ + g_ + r_) / 3;
    }

    T green() const {
        return g_;
    }

    bool operator==(const ColorImpl<T>& c) const {
        return r_ == c.r_ && g_ == c.g_ && b_ == c.b_ && a_ == c.a_;
    }

    bool operator!=(const ColorImpl<T>& c) const {
        return r_ != c.r_ || g_ != c.g_ || b_ != c.b_ || a_ != c.a_;
    }

    T red() const {
        return r_;
    }

    void set(T r, T g, T b) {
        r_ = r;
        g_ = g;
        b_ = b;
    }

    void setAlpha(T a) {
        a_ = a;
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
private:
    T r_, g_, b_, a_;
public:
    static ColorImpl<T> black;
    static ColorImpl<T> white;
    static ColorImpl<T> undefined;
};

template<class T>
ColorImpl<T> ColorImpl<T>::black;

template<class T>
ColorImpl<T> ColorImpl<T>::white(255, 255, 255);

template<class T>
ColorImpl<T> ColorImpl<T>::undefined(255, 255, 255, 255);

template<>
inline int ColorImpl<unsigned char>::get() const {
    return b_ | (g_ << 8) | (r_ << 16);
}

typedef ColorImpl<unsigned char> Color;
}

#endif /* COLOR_H_ */
