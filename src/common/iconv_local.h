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

#ifndef ICONV_H_
#define ICONV_H_

#ifdef __FreeBSD__
#undef __BSD_VISIBLE
#endif

#include <string>
#include <boost/noncopyable.hpp>
#include "globus.h"
#include "lang_def.h"
#include "common/exception.h"

namespace cf
{

class IconvImpl;
class Language;

class CLA_EXPO Iconv : public boost::noncopyable
{
    public:
        /**
          * Created empty iconv conversion
          * To use encoding conversion you should use open() method
          * @see open()
          */
        Iconv();

        /**
          * Creates iconv conversion
          * @param from - source encoding, for example "cp1251"
          * @param to - destination encoding, for example "utf-8"
          * @throw Iconv::Exception if unknown encoding names given
          * @see open()
          */
        Iconv(const std::string& from, const std::string& to);
        ~Iconv();

        typedef RuntimeExceptionImpl<Iconv> Exception;

        /**
          * Closes encoding conversion
          * @return true on success, false on error
          * @see open(), isOpened()
          */
        bool close();

        /**
          * Converts given string
          * @param src - source string
          * @return converted string
          */
        std::string convert(const std::string& src);

        /**
          * Converts given character
          * @param c - source character
          * @return converted character
          */
        std::string convert(unsigned char c);

        /**
          * Checks if conversion is opened
          * @return true if conversion is opened
          * @see open(), close()
          */
        bool isOpened() const;

        /**
          * Opens encoding conversion
          * @param from - source encoding
          * @param to - destination encoding
          * @return true on success, false on error
          * @see isOpened(), close()
          */
        bool open(const std::string& from, const std::string& to);

        /**
          * Opens encoding for given language to utf-8
          * @param from - source language
          * @see isOpened(), close()
          */
        bool openToUtf8(language_t lang);
        bool openToUtf8(const Language& lang);
    private:
        IconvImpl * impl_;
};

}

#endif /* ICONV_H_ */
