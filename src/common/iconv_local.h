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
#include "common/exception.h"

namespace cf
{

class IconvImpl;

class FUN_EXPO__ Iconv : public boost::noncopyable
{
    public:
        Iconv();
        Iconv(const std::string &from, const std::string &to);
        ~Iconv();

        typedef RuntimeExceptionImpl<Iconv> Exception;

        bool close();
        std::string convert(const std::string& src);
        bool open(const std::string &from, const std::string &to);
    private:
        IconvImpl * impl_;
};

}

#endif /* ICONV_H_ */
