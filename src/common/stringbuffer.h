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

#ifndef STRINGBUFFER_H
#define STRINGBUFFER_H

#include <iosfwd>
#include <sstream>
#include <string>

#include "globus.h"

namespace cf {

class CLA_EXPO StringBuffer
{
    StringBuffer(StringBuffer&);
    void operator=(const StringBuffer&);
public:
    StringBuffer();

    template<class T>
    StringBuffer& operator<<(const T& v)
    {
        if(buf_.tellp() != 0)
            buf_ << ' ';

        buf_ << v;
        return *this;
    }

    operator std::string();
private:
    std::ostringstream buf_;
};

typedef StringBuffer StrBuf;

}

#endif // STRINGBUFFER_H
