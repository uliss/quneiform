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

#include "pagestorage.h"

namespace cf {

PageStorage::PageStorage()
{
}

PageStorage& PageStorage::instance()
{
    static PageStorage s;
    return s;
}

PageList& PageStorage::pages()
{
    return instance().pages_;
}

Handle PageStorage::append(BACKUPPAGE &p)
{
    return pages().AddTail(p);
}

Handle PageStorage::backupPage(Handle p)
{
    return page(p).BackUp();
}

void PageStorage::clear()
{
    pages().Clear();
}

void PageStorage::clearPage(Handle p)
{
    page(p).Clear();
}

BACKUPPAGE& PageStorage::page(Handle p)
{
    return pages().GetItem(p);
}

BACKUPPAGE& PageStorage::pageAt(size_t pos)
{
    return page(pageHandleAt(pos));
}

Handle PageStorage::pageHandleAt(size_t pos)
{
    return pages().GetHandle(pos);
}

Handle PageStorage::pageType(Handle p)
{
    return page(p).GetType();
}

size_t PageStorage::pagePosition(Handle p)
{
    return pages().GetPos(p);
}

size_t PageStorage::size()
{
    return pages().GetCount();
}

void PageStorage::remove(Handle p)
{
    pages().Del(p);
}

bool PageStorage::undo(Handle p, Handle num)
{
    return page(p).Undo(num);
}

}
