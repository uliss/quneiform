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

#include <errno.h>
#include <cassert>
#include "iconv_local.h"
#include "iconvimpl.h"

namespace cf
{

Iconv::Iconv() {
    impl_ = new IconvImpl;
}

Iconv::Iconv(const std::string& from, const std::string& to) {
    impl_ = new IconvImpl(from, to);
}

Iconv::~Iconv() {
    delete impl_;
}

bool Iconv::close() {
    assert(impl_);

    return impl_->close();
}

std::string Iconv::convert(unsigned char c) {
    assert(impl_);

    return impl_->convert(c);
}

std::string Iconv::convert(const std::string& src) {
    assert(impl_);

    return impl_->convert(src);
}

bool Iconv::isOpened() const {
    assert(impl_);

    return impl_->isOpened();
}

bool Iconv::open(const std::string &from, const std::string &to) {
    assert(impl_);

    return impl_->open(from, to);
}

}
