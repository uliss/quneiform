/*
 * size.h
 *
 *  Created on: 15.10.2009
 *      Author: uliss
 */

#ifndef SIZE_H_
#define SIZE_H_

namespace CIF {

template<class T>
class SizeImpl
{
public:
    SizeImpl() :
        width_(0), height_(0) {
    }

    SizeImpl(T width, T height) :
        width_(width), height_(height) {
    }

    T height() const {
        return height_;
    }

    template<class U>
    bool operator==(const SizeImpl<U>& sz) {
        return width_ == sz.width() && height_ == sz.height();
    }

    template<class U>
    bool operator!=(const SizeImpl<U>& sz) {
        return width_ != sz.width() || height_ != sz.height();
    }

    template<class U>
    void operator=(const SizeImpl<U>& sz) {
        setWidth(sz.width());
        setHeight(sz.height());
    }

    T& rheight() {
        return height_;
    }

    T& rwidth() {
        return width_;
    }

    SizeImpl& scale(double factor) {
        width_ *= factor;
        height_ *= factor;
        return *this;
    }

    void setHeight(T h) {
        height_ = h;
    }

    void setWidth(T w) {
        width_ = w;
    }

    T width() const {
        return width_;
    }

private:
    T width_, height_;
};

template<class T, class U>
bool SizeContentEqual(const SizeImpl<T>& s0, const SizeImpl<U>& s1) {
    return s0.height() * s0.width() == s1.height() * s1.width();
}

typedef SizeImpl<unsigned int> Size;

}

#endif /* SIZE_H_ */
