/***************************************************************************
 *   Copyright (C) 2011 by Serge Poltavsky                                 *
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

#ifndef MACCFTYPE_H
#define MACCFTYPE_H

#include <CoreFoundation/CFBase.h>

template <typename T>
class MacCFType
{
public:
    MacCFType(const T& t = 0) : t_(t) {
    }

    MacCFType(const MacCFType &helper)
        : t_(helper.t_)
    {
        if (t_)
            CFRetain(t_);
    }

    ~MacCFType()
    {
        if(t_)
            CFRelease(t_);
    }

    operator T()
    {
        return t_;
    }

    MacCFType operator=(const MacCFType& other)
    {
        if (other.t_)
            CFRetain(other.t_);
        CFTypeRef type2 = t_;
        t_ = other.t_;
        if (type2)
            CFRelease(type2);
        return *this;
    }

    T * operator&() {
        return &t_;
    }

    static MacCFType constructFromGet(const T& t)
    {
        CFRetain(t);
        return MacCFType<T>(t);
    }
protected:
    T t_;
};

#endif // MACCFTYPE_H
