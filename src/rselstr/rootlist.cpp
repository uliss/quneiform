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

#include <stdlib.h>
#include <assert.h>

#include "rootlist.h"
#include "roots_struct.h"
#include "common/singleton.h"

namespace cf {

typedef Singleton<RootList> SRootList;

RootList::RootList() :
    roots_(0),
    count_(0),
    capacity_(0)
{
}

RootList::~RootList()
{
    free();
}

bool RootList::add(const ROOT& r)
{
    if(isNull())
        reserve(10);

    if(count_ >= capacity_)
        reserve(100);

    count_++;
    roots_[count_ - 1] = r;
    return true;
}

size_t RootList::count() const
{
    return count_;
}

bool RootList::isEmpty() const
{
    return count_ == 0;
}

bool RootList::isNull() const
{
    return roots_ == 0;
}

ROOT * RootList::at(size_t pos)
{
    if(isNull())
        return 0;

    if(pos >= count())
        return 0;

    return &roots_[pos];
}

ROOT * RootList::first()
{
    if(isNull())
        return 0;

    return isEmpty() ? 0 : roots_;
}

ROOT * RootList::last()
{
    if(isNull())
        return 0;

    return isEmpty() ? 0 : &roots_[count_ - 1];
}

ROOT * RootList::reserve(size_t num)
{
    size_t mem_size = capacity_ + num * sizeof(ROOT);
    roots_ = (ROOT*) realloc(roots_, mem_size);

    assert(roots_);

    capacity_ += num;
    return roots_;
}

size_t RootList::capacity() const
{
    return capacity_;
}

void RootList::clear()
{
    count_ = 0;
}

void RootList::free()
{
    ::free(roots_);
    roots_ = 0;
    count_ = 0;
    capacity_ = 0;
}

bool Roots::add(const ROOT& r)
{
    return SRootList::instance().add(r);
}

ROOT * Roots::at(size_t pos)
{
    return SRootList::instance().at(pos);
}

size_t Roots::count()
{
    return SRootList::instance().count();
}

ROOT * Roots::first()
{
    return SRootList::instance().first();
}

ROOT * Roots::last()
{
    return SRootList::instance().last();
}

void Roots::free()
{
    SRootList::instance().free();
}

bool Roots::isEmpty()
{
    return SRootList::instance().isEmpty();
}

bool Roots::reserve(size_t num)
{
    return SRootList::instance().reserve(num);
}


}
