/***************************************************************************
 *   Copyright (C) 2010 by Serge Poltavsky                                 *
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
#include "iconvimpl.h"
#include "iconv_local.h"

namespace CIF
{

IconvImpl::IconvImpl() :
    iconv_((iconv_t) -1) {

}

IconvImpl::IconvImpl(const std::string &from, const std::string &to) {
    if (!open(from, to))
        throw Iconv::Exception("Can't convert from " + from + " to " + to);
}

IconvImpl::~IconvImpl() {
    close();
}

bool IconvImpl::close() {
    bool result = true;

#ifdef CF_USE_ICONV
    if (iconv_ != iconv_t(-1)) {
        if (::iconv_close(iconv_) == -1)
        result = false;

        iconv_ = iconv_t(-1);
    }
#endif

    return result;
}

void throwException() {
#ifdef CF_USE_ICONV
    switch (errno) {
        case E2BIG:
        break;
        case EILSEQ:
        throw Iconv::Exception("Invalid character or multibyte sequence in the input");
        break;
        case EINVAL:
        default:
        throw Iconv::Exception("Incomplete multibyte sequence in the input");
        break;
    }
#endif
}

std::string IconvImpl::convert(const std::string& src) {
#ifdef CF_USE_ICONV
    std::string result;

    if (src.empty())
    return result;

    char output_buf[1024];
    const char *source_ptr = src.c_str();
    size_t source_len = src.length();
    size_t output_len = sizeof(output_buf) - sizeof(output_buf[0]);

    while (source_len > 0) {
        output_buf[0] = '\0';
        char * output_ptr = output_buf;

        if (convert(&source_ptr, &source_len, &output_ptr, &output_len) == size_t(-1))
        throwException();

        *output_ptr = '\0';
        result += output_buf;
    }

    return result;
#endif
    return src;
}

size_t IconvImpl::convert(const char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft) {
#ifdef CF_USE_ICONV
    return ::iconv(iconv_,
#if !defined(ICONV_SECOND_ARGUMENT_IS_CONST)
            (char**)
#endif
            inbuf, inbytesleft, outbuf, outbytesleft);
#else
    // see man 3 iconv regarding return value
    return static_cast<size_t>(-1);
#endif
}

bool IconvImpl::open(const std::string &from, const std::string &to) {
#ifdef CF_USE_ICONV
    close();
    iconv_ = ::iconv_open(to.c_str(), from.c_str());
    return iconv_ != iconv_t(-1);
#endif
    return true;
}

}
