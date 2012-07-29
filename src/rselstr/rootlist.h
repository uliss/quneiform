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

#ifndef ROOTLIST_H
#define ROOTLIST_H

#include <stddef.h>

#include "globus.h"

struct ROOT;

namespace cf {

class CLA_EXPO RootList
{
public:
    RootList();
    ~RootList();

    bool add(const ROOT& r);
    ROOT * at(size_t pos);
    ROOT * first();
    ROOT * last();
    ROOT * reserve(size_t num);

    size_t capacity() const;
    void clear();
    void free();
    size_t count() const;
    bool isEmpty() const;
    bool isNull() const;
private:
    ROOT * roots_;
    size_t count_;
    size_t capacity_;
};

class Roots
{
    Roots();
    Roots(const Roots&);
public:
    static bool add(const ROOT& r);
    static ROOT * at(size_t pos);
    static size_t count();
    static ROOT * first();
    static ROOT * last();
    static void free();
    static bool isEmpty();
    static bool reserve(size_t num);
};

}

#endif // ROOTLIST_H
