/*
 * color.h
 *
 *  Created on: 10.10.2009
 *      Author: uliss
 */

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
