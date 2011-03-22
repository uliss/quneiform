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
#include <iostream>
#include "iconvimpl.h"
#include "iconv_local.h"

namespace cf
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

std::string IconvImpl::convert(unsigned char chr) {
#ifdef CF_USE_ICONV
    if(chr < '\x7E') // ascii char
        return std::string(1, chr);

    char src[] = { chr };
    char * src_ptr = src;
    size_t src_len = sizeof(char);
    char dest[8] = {0};
    char * dest_ptr = dest;
    size_t dest_len = sizeof(dest);
    if(convert(&src_ptr, &src_len, &dest_ptr, &dest_len) == size_t(-1)) {
        switch (errno) {
        case EILSEQ:
            throw Iconv::Exception(
                    "[IconvImpl::convert] invalid character or multibyte sequence in the input");
            break;
        case EINVAL:
            throw Iconv::Exception(
                    "[IconvImpl::convert] incomplete multibyte sequence in the input");
            break;
        default:
            throw Iconv::Exception("[IconvImpl::convert] failed");
            break;
        }
    }

    *dest_ptr = '\0';

    return std::string(dest);
#else
    return std::string(1, chr);
#endif
}

std::string IconvImpl::convert(const std::string& src) {
#ifdef CF_USE_ICONV
    if (src.empty())
        return src;

    std::string result;
    result.reserve(src.size());

    char * source_ptr = (char*) src.c_str();
    size_t source_len = src.length();

    static const int BUFSIZE = 4096;
    static const int NUL_TERMINATOR_LENGTH = 4;
    char output_buf[BUFSIZE];
    size_t output_len = BUFSIZE - NUL_TERMINATOR_LENGTH;

    while (source_len > 0) {
        char * output_ptr = output_buf;

        if (convert(&source_ptr, &source_len, &output_ptr, &output_len) == (size_t) -1) {
            switch (errno) {
            case E2BIG:
                output_len = BUFSIZE - NUL_TERMINATOR_LENGTH;
                break;
            case EILSEQ:
                throw Iconv::Exception(
                        "[IconvImpl::convert] invalid character or multibyte sequence in the input");
                break;
            case EINVAL:
            default:
                throw Iconv::Exception(
                        "[IconvImpl::convert] incomplete multibyte sequence in the input");
                break;
            }
        }

        *output_ptr = '\0';
        result.append(output_buf);
    }

    return result;
#else
    return src;
#endif
}

size_t IconvImpl::convert(char **inbuf, size_t *inbytesleft, char **outbuf, size_t *outbytesleft) {
#ifdef CF_USE_ICONV
#ifdef ICONV_SECOND_ARGUMENT_IS_CONST
    return ::iconv(iconv_, (const char**) inbuf, inbytesleft, outbuf, outbytesleft);
#else
    return ::iconv(iconv_, inbuf, inbytesleft, outbuf, outbytesleft);
#endif
#else
    // see man 3 iconv regarding return value
    return static_cast<size_t> (-1);
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
