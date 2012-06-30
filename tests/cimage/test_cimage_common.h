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

#ifndef TEST_CIMAGE_COMMON_H
#define TEST_CIMAGE_COMMON_H

#include <cppunit/extensions/HelperMacros.h>
#include <string>

#include "cttypes.h"
#include "common/ctdib.h"
#include "rdib/bmpimageloader.h"
#include "cimage/imageinfo.h"

#define IS_WHITE_DIB32(dib, x, y) {\
    uint32_t * pixel = (uint32_t*) dib->pixelAt(x, y);\
    CPPUNIT_ASSERT(pixel);\
    CPPUNIT_ASSERT_EQUAL((uint32_t) 0xffffffff, *pixel);\
    }

#define IS_BLACK_DIB32(dib, x, y) {\
    uint32_t * pixel = (uint32_t*) dib->pixelAt(x, y);\
    CPPUNIT_ASSERT(pixel);\
    CPPUNIT_ASSERT_EQUAL((uint32_t) 0x0, *pixel);\
    }

#define IS_WHITE_DIB1(dib, x, y) {\
    uchar * pixel = (uchar*) dib->pixelAt(x, y);\
    CPPUNIT_ASSERT(pixel);\
    CPPUNIT_ASSERT(*pixel & (0x80 >> (x % 8)));\
    }

#define IS_BLACK_DIB1(dib, x, y) {\
    uchar * pixel = (uchar*) dib->pixelAt(x, y);\
    CPPUNIT_ASSERT(pixel);\
    CPPUNIT_ASSERT(!(*pixel & (0x80 >> (x % 8))));\
    }

#define IS_WHITE_HANDLE_32(handle, x, y) {\
    CTDIB dib;\
    dib.setBitmap(handle);\
    IS_WHITE_DIB32((&dib), x, y);\
    }

#define IS_WHITE_HANDLE_1(handle, x, y) {\
    CTDIB dib;\
    dib.setBitmap(handle);\
    IS_WHITE_DIB1((&dib), x, y);\
    }

#define IS_BLACK_HANDLE_1(handle, x, y) {\
    CTDIB dib;\
    dib.setBitmap(handle);\
    IS_BLACK_DIB1((&dib), x, y);\
    }


// note: caller should detele[] result
static inline cf::BitmapPtr loadDibFromBmp(const std::string& name) {
    return (cf::BitmapPtr) cf::BmpImageLoader().loadHandle(LOADER_TEST_IMAGE_DIR + name);
}

class DibPtr {
public:
    DibPtr(cf::CTDIB * dib) : dib_(dib) {}
    DibPtr(const std::string& name) : dib_(NULL) {
        dib_ = new cf::CTDIB;

        if(!dib_->setBitmap(loadDibFromBmp(name))) {
            delete dib_;
            dib_ = NULL;
        }
    }

    ~DibPtr() {
        if(dib_) {
            cf::BitmapPtr h = NULL;
            dib_->bitmap(&h);
            delete[] (cf::BitmapPtr) h;
        }

        delete dib_;
    }

    cf::BitmapPtr handle() {
        if(!dib_)
            return NULL;

        cf::BitmapPtr h = NULL;
        dib_->bitmap(&h);
        return h;
    }

    operator cf::CTDIB* () { return dib_; }
    cf::CTDIB * operator->() { return dib_; }
    operator bool () const { return dib_ != NULL; }
private:
    cf::CTDIB * dib_;
};

#endif // TEST_CIMAGE_COMMON_H
