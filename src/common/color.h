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

namespace CIF
{

template<class T>
class ColorImpl
{
        ColorImpl(T red, T green, T blue) :
                r_(red), g_(green), b_(blue), a_(0) {
        }

        T alpha() const {
            return a_;
        }

        T blue() const {
            return b_;
        }

        T gray() const {
            return (b_ + g_ + r_) / 3;
        }

        T green() const {
            return g_;
        }

        T red() const {
            return r_;
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
};

typedef ColorImpl<unsigned char> Color;
}

#endif /* COLOR_H_ */
